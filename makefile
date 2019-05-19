#	avrsynth - simple AVR synthesizer project
#	Copyright (C) 2019 Jacek Wieczorek <mrjjot@gmail.com>
#	This file is part of liblightmodbus.
#	Avrsynth is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#	Avrsynth is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.

F_CPU = 20000000UL
SAMPLE_LEN = 32
MCU = atmega32

CC = avr-gcc
CFLAGS = -Wall -Os

all: clean force bin/synth.elf
	
bin/synth.elf: src/synth.c src/osc.c src/modulation.c src/envelope.c src/midi.c src/com.c src/notes.c
	$(CC) $(CFLAGS) -DSAMPLE_LEN=$(SAMPLE_LEN) -DF_CPU=$(F_CPU) -DNOTE_LIM=$(NOTE_LIM) -mmcu=$(MCU) $^ -o $@
	avr-size -C $@ --mcu=$(MCU)
	
force:
	-mkdir bin

clean:
	-rm -rf bin
	
prog: bin/synth.elf
	avrdude -c usbasp -p m32 -U flash:w:$^
