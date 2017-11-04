#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "osc.h"
#include "modulation.h"
#include "envelope.h"
#include "notes.h"
#include "com.h"
#include "midi.h"

void comtxs( const char *s )
{
	while ( *s ) comtx( *s++ );
}

int main( )
{
	//Init synthesizer modules
	oscinit( );
	modinit( );
	cominit( 9600 );
	midi.channel = 0;
	midi.note = 60;
	
	
	ldsample( samples[3], 255 );
	oscset( pgm_read_word( notes + 50 ) );
	sei( );
	
	DDRB = 7;
	
	
	while ( 1 )
	{
		if ( comstatus( ) )
			midiproc( UDR );
		
		oscset( pgm_read_word( &notes[midi.note] ) );
	}
	
	return 0;
}
