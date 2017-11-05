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
