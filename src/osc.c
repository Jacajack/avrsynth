#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "envelope.h"
#include "hmul.h"
#include "osc.h"

//Samples
#ifndef SAMPLE_LEN
#error SAMPLE_LEN must be defined in order to load proper sample table!
#else
const static volatile uint8_t PROGMEM samples[][SAMPLE_LEN] =
#endif
#if SAMPLE_LEN == 16
{
	{  0,   0,   0,   0,  0,   0,    0,   0, 255, 255, 255, 255, 255, 255, 255, 255}, //Square wave
	{  0,  16,  32,  48,  64, 80,   96, 112, 128, 144, 160, 176, 192, 208, 224, 240}, //Saw wave
	{  0,  10,  37,  78, 127, 176, 217, 244, 254, 244, 217, 176, 127,  78,  37,  10}, //Sine wave
	{128, 160, 192, 224, 255, 224, 192, 160, 128,  96,  64,  32,   0,  32,  64,  96}, //Triangle
	{255, 229, 207, 194, 191, 200, 217, 242,  13,  38,  55,  64,  61,  48,  26,   0}, //Some ATARI wave I'm trying to recreate

};
#endif
#if SAMPLE_LEN == 32
{
	{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //Square wave
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, 
	 
	{  0,   8,  16,  24,  32,  40,  48,  56,  64,  72,  80,  88,  96, 104, 112, 120,  //Saw wave
	 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248},
	 
	
	{127, 152, 176, 198, 217, 233, 244, 252, 254, 252, 244, 233, 217, 198, 176, 152,  //Sine wave
	 127, 102,  78,  56,  31,  21,  10,   2,   0,   2,  10,  21,  37,  56,  78, 102},
	 
	{  0,  16,  32,  48,  64,  80,  96, 112, 128, 144, 160, 176, 192, 208, 224, 240,  //Triangle wave
	 240, 224, 208, 192, 176, 160, 144, 128, 112,  96,  80,  64,  48,  32,  16,   0},
	 
	 
	{  0,   4,   8,  12,  16,  20,  24,  28,  32,  36,  40,  44,  48,  52,  56,  62,  //The weird ATARI wave
	 255, 240, 225, 211, 199, 190, 182, 177, 175, 176, 180, 185, 193, 203, 214, 225},
	 
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  //PWM 1/8
	 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128,   0,   0,   0,   0, 128}, 
};
#endif

//Oscillator variables
volatile static uint8_t samplenum, noise;
volatile static uint8_t buffers[2][SAMPLE_LEN] = {{0}};
volatile static uint8_t *outbuf = buffers[0], *genbuf = buffers[1];

static inline uint16_t noisegen( uint8_t vol )
{
	static uint16_t lfsr = 0xace1u;
	static uint8_t bit;
	
	bit = ( ( lfsr >> 0 ) ^ ( lfsr >> 2 ) ^ ( lfsr >> 3 ) ^ ( lfsr >> 5 ) ) & 1;
	lfsr = ( lfsr >> 1 ) | ( bit << 15 );
	return ( ( lfsr & 0xff ) * vol ) >> 8;
} 

//The main sound generation interrupt
ISR ( TIMER1_COMPA_vect )
{
	PORTA = outbuf[samplenum] + noisegen( noise );
	if ( ++samplenum >= SAMPLE_LEN ) samplenum = 0;
}

//Set noise volume
void oscnoise( uint8_t vol )
{
	noise = vol;
}

//Load sample (or change volume)
void ldsample( uint8_t num, uint8_t vol )
{
	const volatile static uint8_t *sample;
	volatile static uint8_t *bufswap;
	static uint8_t i;

	sample = samples[num];
	
	/*
	//Multiply the wave with volume (inline assembly! yay!)
	for ( i = 0; i < SAMPLE_LEN; i++ )
		HMUL( genbuf[i], pgm_read_byte( sample + i ), vol );
	*/
	
	//Multiply the sample with proper volume
	for ( i = 0; i < SAMPLE_LEN; i++ )
		genbuf[i] = ( ( (uint16_t) pgm_read_byte( sample + i ) * vol ) >> 8 );
	
	
	//Swap buffers
	bufswap = outbuf;
	outbuf = genbuf;
	genbuf = bufswap;
	
}

//Set oscillator comparator value
void oscset( uint16_t compv )
{
	OCR1A = compv;
	if ( TCNT1 > compv ) TCNT1 = 0;
}

//Init osicllator
void oscinit( )
{
	//DAC as output
	DDRA = 0xff;
	PORTA = 0;

	//Timer/counter 1 config
	//Running at F_CPU
	TCCR1A = 0;
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS10 );
	TCNT1 = 0;
	OCR1A = 65535;
	TIMSK |= ( 1 << OCIE1A );
}

