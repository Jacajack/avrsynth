#ifndef ENVELOPE_H
#define ENVELOPE_H
#include <inttypes.h>

struct envgen
{
	uint8_t attack; //Attack
	uint8_t sustain; //Sustain
	uint8_t release; //Release
	uint8_t value; //Value
	uint8_t keydn; //Is key pressed
};

extern void envupd( struct envgen *gen );

#endif
