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

#ifndef HMUL_H
#define HMUL_H

//Hardware multiplication routines

#define HMUL( ans, b1, b2 ) \
	asm volatile( \
		"mul %1, %2\n\t" \
		"mov %0, r1\n\t" \
		: "=l"(ans) \
		: "d"(b1), "d"(b2) \
	);
	
#define HMULS( ans, b1, b2 ) \
	asm volatile( \
		"muls %1, %2\n\t" \
		"mov %0, r1\n\t" \
		: "=l"(ans) \
		: "d"(b1), "d"(b2) \
	);

#endif
