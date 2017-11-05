#ifndef OSC_H
#define OSC_H

extern void oscinit( );
extern void oscset( uint16_t compv );
extern inline void oscnoise( uint8_t vol );
extern inline void oscmute( uint8_t enable );
extern void ldsample( uint8_t num, uint8_t vol );


#endif
