#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "envelope.h"
#include "hmul.h"
#include "osc.h"

//Samples
const volatile uint8_t PROGMEM samples[SAMPLE_COUNT][SAMPLE_LEN] = 
{
	{  0,   0,   0,   0,  0,   0,    0,   0, 255, 255, 255, 255, 255, 255, 255, 255}, //Square wave
	{  0,  16,  32,  48,  64, 80,   96, 112, 128, 144, 160, 176, 192, 208, 224, 240}, //Saw wave
	{  0,  10,  37,  78, 127, 176, 217, 244, 254, 244, 217, 176, 127,  78,  37,  10}, //Sine wave
	{128, 160, 192, 224, 255, 224, 192, 160, 128,  96,  64,  32,   0,  32,  64,  96}, //Triangle
	{255, 229, 207, 194, 191, 200, 217, 242,  13,  38,  55,  64,  61,  48,  26,   0}, //Some ATARI wave I'm trying to recreate

};

//Oscillator variables
volatile static uint8_t samplenum;
volatile static uint8_t buffers[2][SAMPLE_LEN] = {{0}};
volatile static uint8_t *outbuf = buffers[0], *genbuf = buffers[1];

//The main sound generation interrupt
ISR ( TIMER1_COMPA_vect )
{
	PORTA = outbuf[samplenum];
	if ( ++samplenum >= 16 ) samplenum = 0;
}

//Load sample (or change volume)
void ldsample( const volatile uint8_t *sample, uint8_t vol )
{
	volatile static uint8_t *bufswap;
	static uint8_t i;
	
	/*
	//Multiply the wave with volume (inline assembly! yay!)
	for ( i = 0; i < SAMPLE_LEN; i++ )
		HMUL( genbuf[i], pgm_read_byte( sample + i ), vol );
	*/
	
	//Multiply the sample with proper volume
	for ( i = 0; i < SAMPLE_LEN; i++ )
		genbuf[i] = ( ( (uint16_t) pgm_read_byte( sample + i ) * vol ) >> 8 );
	
	
	//Buffer swap
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

