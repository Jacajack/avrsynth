#ifndef OSC_H
#define OSC_H
#include <avr/pgmspace.h>

#ifndef SAMPLE_LEN
#error SAMPLE_LEN must be defined in order to load proper sample table!
#else
extern const volatile uint8_t PROGMEM samples[][SAMPLE_LEN];
#define SAMPLE_COUNT ( sizeof( samples ) / sizeof( samples[0] ) )
#endif

extern volatile uint8_t noteon, notenum;

extern void oscinit( );
extern void oscset( uint16_t compv );
extern void ldsample( const volatile uint8_t *sample, uint8_t vol );


#endif
