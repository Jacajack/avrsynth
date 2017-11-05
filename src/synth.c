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

//Midi channels
struct midistatus midi0 = {0}, midi10 = {0};

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
	uint8_t skipping = 0;

	//LED init
	DDRB = 7;

	//Init synthesizer modules
	oscinit( );
	modinit( );
	cominit( 31250 );
	
	//Init EGs
	envgen0.attack = 0xffff;
	envgen0.sustain = 0xffff;
	envgen0.release = 0xffff;
	envgen1.attack = 0xffff;
	envgen1.sustain = 0xffff;
	envgen1.release = 0xffff;
	
	//Enable interrupts
	sei( );
	
	while ( 1 )
	{
		//Receive MIDI command
		if ( comstatus( ) )
			midiproc( &midi0, UDR, 0 );
			
		//Handle MIDI reset	
		if ( midi0.reset ) reset( );
		
		//Update EG1 params
		envgen0.sustain = midi0.notevel << 9;
		envgen0.attack = ( 128 - midi0.controllers.sndctl4 ) << 7;
		envgen0.release = ( 128 - midi0.controllers.sndctl3 ) << 7;
		envgen0.keydn = midi0.noteon;
		
		//Update EG2 params
		envgen1.keydn = midi0.noteon;
		
		//Update oscillator params
		ldsample( midi0.program, envgen0.value );
		
		//Update comparator value - hybrid sampling rate switching
		skipping = 0;
		if ( midi0.note > 72 ) skipping = 1;
		if ( midi0.note > 84 ) skipping = 2;
		if ( midi0.note > 90 ) skipping = 3;
		oscset( pgm_read_word( &notes[midi0.note] ), skipping );

			
		//Light up LED's proportionally to EG1, EG2 and LFO
		ledpwm( envgen0.value, envgen1.value, 128 );
	}
	
	return 0;
}
