#include <avr/io.h>
#include <util/delay.h>

int main( )
{
	//Simple blink
	DDRB = 1;
	while ( 1 )
	{
		_delay_ms( 200 );
		PORTB ^= 1;
	}
	
	return 0;
}
