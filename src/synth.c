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

//Reset routine
void reset( )
{
	wdt_enable( WDTO_15MS );
	while ( 1 );
}

//Disable watchdog after reset
void __attribute__( ( naked ) ) __attribute__( ( section( ".init3" ) ) ) watchdogInit( ) 
{
	MCUSR = 0;
	wdt_disable( );
}

//LED control routine
inline void ledpwm( uint8_t l1, uint8_t l2, uint8_t l3 )
{
	static uint8_t pwmcnt = 0;
	
	if ( pwmcnt < l1 ) PORTB |= ( 1 << 0 );
	else PORTB &= ~( 1 << 0 );
	
	if ( pwmcnt < l2 ) PORTB |= ( 1 << 1 );
	else PORTB &= ~( 1 << 1 );
	
	if ( pwmcnt < l3 ) PORTB |= ( 1 << 2 );
	else PORTB &= ~( 1 << 2 );
	
	pwmcnt++;
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
	
	envgen0.attack = 255;
	envgen0.sustain = 255;
	envgen0.release = 20;
	
	while ( 1 )
	{
		//Receive MIDI command
		if ( comstatus( ) )
			midiproc( UDR );
			
		//Handle MIDI reset	
		if ( midi.reset ) reset( );
		
		//Update EG1 params
		envgen0.sustain = midi.notevel;
		envgen0.attack = 255 - midi.controllers.sndctl4 * 2;
		envgen0.release = 255 - midi.controllers.sndctl3 * 2;
		envgen0.keydn = midi.noteon;
		
		//Update oscillator params
		ldsample( samples[midi.program], envgen0.value );
		oscset( pgm_read_word( &notes[midi.note] ) );
		
		//Light up LED's proportionally to EG1, EG2 and LFO
		ledpwm( envgen0.value, envgen1.value, 128 );
	}
	
	return 0;
}
