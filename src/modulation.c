/*
	avrsynth - simple AVR synthesizer project
	Copyright (C) 2019 Jacek Wieczorek <mrjjot@gmail.com>
	This file is part of liblightmodbus.
	Avrsynth is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Avrsynth is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

