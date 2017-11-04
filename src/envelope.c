#include <inttypes.h>
#include "envelope.h"

void envupd( struct envgen *gen )
{
	int16_t step;
	
	if ( gen->keydn ) //Attack
	{
		step = gen->value + gen->attack;
		if ( step > gen->sustain ) gen->value = gen->sustain;
		else gen->value = step;
	}
	else //Release
	{
		step = gen->value - gen->release;
		if ( step < 0 ) gen->value = 0;
		else gen->value = step;
	}
}
