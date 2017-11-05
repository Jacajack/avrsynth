#include <avr/io.h>
#include <avr/wdt.h>
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

void reset( )
{
	wdt_enable( WDTO_15MS );
	while ( 1 );
}

void __attribute__( ( naked ) ) __attribute__( ( section( ".init3" ) ) ) watchdogInit( ) 
{
	MCUSR = 0;
	wdt_disable( );
}

int main( )
{
	//Init synthesizer modules
	oscinit( );
	modinit( );
	cominit( 31250 );
	midiinit( 0 );
	sei( );
	
	DDRB = 7;
	
	envgen1.attack = 255;
	envgen1.sustain = 255;
	envgen1.release = 20;
	
	while ( 1 )
	{
		if ( comstatus( ) )
			midiproc( UDR );
		
		envgen1.keydn = midi.noteon;
		ldsample( samples[midi.program], envgen1.value );
		oscset( pgm_read_word( &notes[midi.note] ) );
		envgen1.sustain = midi.notevel;
		envgen1.attack = 255 - midi.controllers.sndctl4 * 2;
		envgen1.release = 255 - midi.controllers.sndctl3 * 2;
		
		if ( midi.reset ) reset( );
	}
	
	return 0;
}
