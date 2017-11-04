#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "osc.h"
#include "modulation.h"
#include "envelope.h"
#include "notes.h"

int main( )
{
	//Init synthesizer modules
	oscinit( );
	modinit( );
	
	ldsample( samples[3], 128 );
	OCR1A = pgm_read_word( notes + 50 );
	sei( );
	
	while ( 1 );
	
	return 0;
}
