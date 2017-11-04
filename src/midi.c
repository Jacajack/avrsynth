#include <avr/io.h>
#include <inttypes.h>
#include "midi.h"

struct midistatus midi = {0};

void midiproc( uint8_t byte )
{
	static uint8_t channel = 0, status = 0, dlim = 0, dread = 0;
	static uint8_t dbuf[16] = {0};
		
	if ( byte & ( 1 << 7 ) ) //Handle status bytes
	{
		//Extract information from status byte
		status = byte & 0x70;
		dread = dlim = 0;
		channel = byte & 0x0f;

		//Check data length for each MIDI command
		switch ( status )
		{
			//Note on
			case 0x10:
				dlim = 2;
				break;
				
			//Note off
			case 0x00:
				dlim = 2;
				break;
				
			//Program change
			case 0x40:
				dlim = 1;
				break;
				
			//Pitch
			case 0x60:
				dlim = 2;
				break;
			
			//Uknown command
			default:
				dlim = 0;
				break;
				
		}
	}
	else if ( midi.channel == channel ) //Handle data bytes
	{
		//Data byte
		dbuf[dread++] = byte;
		
		//Interpret command
		if ( dread == dlim )
		{
			switch ( status )
			{
				//Note on
				case 0x10:
					midi.note = dbuf[0];
					midi.noteon = 1;
					break;
					
				//Note off	
				case 0x00:
					//if ( midi.note == dbuf[0] )
						midi.noteon = 0;
					break;
					
				//Program change
				case 0x40:
					midi.program = dbuf[0];
					break;
					
				//Pitch
				case 0x60:
					midi.pitchbend = dbuf[0] | ( dbuf[1] << 7 );
					break;
					
				default:
					break;
			}
			
			dread = 0;
		}
		
	}
}
