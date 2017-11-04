#ifndef OSC_H
#define OSC_H
#include <avr/pgmspace.h>

#define SAMPLE_LEN 16
#define SAMPLE_COUNT 4

extern const volatile PROGMEM uint8_t samples[][SAMPLE_LEN];
extern volatile uint8_t noteon, notenum;

extern void oscinit( );
extern void ldsample( const volatile uint8_t *sample, uint8_t vol );


#endif
