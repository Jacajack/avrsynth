#ifndef SAMPLES_H
#define SAMPLES_H
#include <inttypes.h>

#define SAMPLE_LEN 16

const volatile PROGMEM uint8_t samples[][SAMPLE_LEN] = 
{
	{0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255}, //Square wave
	{0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240}, //Saw wave
	{0, 10, 37, 78, 127, 176, 217, 244, 254, 244, 217, 176, 127, 78, 37, 10}, //Sine wave
	{128, 160, 192, 224, 255, 224, 192, 160, 128, 96, 64, 32, 0, 32, 64, 96}, //Triangle

	
};

#define SAMPLE_COUNT ( sizeof( samples ) / sizeof( samples[0] ) )

#endif
