/*
	avrsynth - simple AVR synthesizer project
	Copyright (C) 2019 Jacek Wieczorek <mrjjot@gmail.com>
	This file is part of liblightmodbus.
	Liblightmodbus is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Liblightmodbus is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ENVELOPE_H
#define ENVELOPE_H
#include <inttypes.h>

struct envgen
{
	uint16_t attack; //Attack
	uint16_t sustain; //Sustain
	uint16_t release; //Release
	union
	{
		uint16_t cnt;
		struct __attribute__( ( packed ) )
		{
			uint8_t cntl;  //Counter low half
			uint8_t value; //Value
		};
	};
	uint8_t keydn; //Is key pressed
};

extern uint8_t envupd( struct envgen *gen );

#endif
