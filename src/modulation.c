#include <avr/interrupt.h>
#include "modulation.h"
#include "envelope.h"

//LFO and envelope variables
struct envgen envgen0, envgen1;

//The LFO and evelope interrupt
ISR ( TIMER0_COMP_vect )
{
	//Update envelope generators
	envupd( &envgen0 );
	envupd( &envgen1 ); 
}

void modinit( )
{
	//Timer/counter 0 config
	//Running at 50Hz
	TCCR0 = ( 1 << WGM01 ) | ( 5 << CS00 );
	TIMSK |= ( 1 << OCIE0 );
	OCR0 = 250;
	TCNT0  = 0;
}

