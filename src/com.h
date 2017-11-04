#ifndef COM_H
#define COM_H
#include <avr/io.h>
#include <inttypes.h>

#define comstatus( ) ( UCSRA & ( 1 << RXC ) )

extern void cominit( uint32_t baud );
extern uint8_t comrecv( );

#endif

