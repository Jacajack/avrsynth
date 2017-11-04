#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "notes.h"
#include "samples.h"
#include "hmul.h"

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


int main( )
{
	DDRA = 0xff;
	
	//Timer/counter 1 config
	//Running at 20MHz
	TCCR1A = 0;
	TCCR1B = ( 1 << WGM12 ) | ( 1 << CS10 );
	TCNT1 = 0;
	TIMSK = ( 1 << OCIE1A );
	
	ldsample( samples[3], 128 );
	OCR1A = pgm_read_word( notes + 50 );
	sei( );
	
	while ( 1 );
	
	return 0;
}
