#include <inttypes.h>
#include "envelope.h"

uint8_t envupd( struct envgen *gen )
{
	int16_t step;
	
	if ( gen->keydn ) //Attack
	{
		step = (int16_t) gen->value + gen->attack;
		if ( step > gen->sustain ) gen->value = gen->sustain;
		else gen->value = step;
	}
	else //Release
	{
		step = (int16_t) gen->value - gen->release;
		if ( step < 0 ) gen->value = 0;
		else gen->value = step;
	}
	
	return gen->value;
}
