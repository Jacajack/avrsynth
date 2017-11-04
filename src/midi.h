#ifndef MIDI_H
#define MIDI_H
#include <inttypes.h>

struct midistatus
{
	uint8_t channel;
	uint8_t program;
	uint8_t noteon;
	uint8_t note;
	uint16_t pitchbend; 
};

extern struct midistatus midi;

extern void midiproc( uint8_t byte );

#endif
