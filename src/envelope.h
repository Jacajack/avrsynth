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
