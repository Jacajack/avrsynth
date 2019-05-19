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

#include <inttypes.h>
#include "envelope.h"

uint8_t envupd( struct envgen *gen )
{
	int32_t step;
	
	if ( gen->keydn ) //Attack
	{
		step = (int32_t) gen->cnt + gen->attack;
		if ( step > gen->sustain ) gen->cnt = gen->sustain;
		else gen->cnt = step;
	}
	else //Release
	{
		step = (int32_t) gen->cnt - gen->release;
		if ( step < 0 ) gen->cnt = 0;
		else gen->cnt = step;
	}
	
	return gen->value;
}
