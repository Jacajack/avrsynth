#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "envelope.h"
#include "hmul.h"
#include "osc.h"

//Samples
const volatile PROGMEM uint8_t samples[SAMPLE_COUNT][SAMPLE_LEN] = 
{
	{  0,   0,   0,   0,  0,   0,    0,   0, 255, 255, 255, 255, 255, 255, 255, 255}, //Square wave
	{  0,  16,  32,  48,  64, 80,   96, 112, 128, 144, 160, 176, 192, 208, 224, 240}, //Saw wave
	{  0,  10,  37,  78, 127, 176, 217, 244, 254, 244, 217, 176, 127,  78,  37,  10}, //Sine wave
	{128, 160, 192, 224, 255, 224, 192, 160, 128,  96,  64,  32,   0,  32,  64,  96}, //Triangle

};


//Oscillator variables
volatile uint8_t noteon, notenum;
volatile static uint8_t samplenum;
volatile static uint8_t buffers[2][SAMPLE_LEN] = {{0}};
volatile static uint8_t *outbuf = buffers[0], *genbuf = buffers[1];

//The main sound generation interrupt
ISR ( TIMER1_COMPA_vect )
{
	PORTA = outbuf[samplenum];
	if ( ++samplenum >= 16 ) samplenum = 0;
}

void ldsample( const volatile uint8_t *sample, uint8_t vol )
{
	volatile static uint8_t *bufswap;
	static uint8_t i;
	
	//Multiply the wave with volume (inline assembly! yay!)
	for ( i = 0; i < SAMPLE_LEN; i++ )
		HMUL( genbuf[i], pgm_read_byte( sample + i ), vol );
	
	//Buffer swap
	bufswap = outbuf;
	outbuf = genbuf;
	genbuf = bufswap;
	
}

void oscinit( )
{
	//DAC as output
	DDRA = 0xff;
	PORTA = 0;

	//Timer/counter 1 config
	//Running at 20MHz
	TCCR1A = 0;
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS10 );
	TCNT1 = 0;
	OCR1A = 65535;
	TIMSK = ( 1 << OCIE1A );
}

