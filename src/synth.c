#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "osc.h"
#include "modulation.h"
#include "envelope.h"
#include "notes.h"
#include "com.h"

int main( )
{
	//Init synthesizer modules
	oscinit( );
	modinit( );
	cominit( 31250 );
	
	
	
	ldsample( samples[3], 128 );
	oscset( pgm_read_word( notes + 50 ) );
	sei( );
	
	DDRB = 1;
	
	while ( 1 )
	{
		comrecv( );
		PORTB ^= 1;
	}
	
	return 0;
}
