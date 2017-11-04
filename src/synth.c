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
	cominit( 31250 );
	midi.channel = 0;
	sei( );
	
	DDRB = 7;
	
	envgen1.attack = 255;
	envgen1.sustain = 255;
	envgen1.release = 2;
	
	while ( 1 )
	{
		if ( comstatus( ) )
			midiproc( UDR );
		
		envgen1.keydn = midi.noteon;
		ldsample( samples[midi.program], envgen1.value );
		oscset( pgm_read_word( &notes[midi.note] ) );
	}
	
	return 0;
}
