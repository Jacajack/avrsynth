#ifndef NOTES_H
#define NOTES_H
#include <inttypes.h>
#include <avr/pgmspace.h>

extern const uint16_t PROGMEM notes[];
#define NOTE_COUNT ( sizeof( notes ) / sizeof( notes[0] ) )

#endif
