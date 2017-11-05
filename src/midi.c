#include <avr/io.h>
#include <inttypes.h>
#include "midi.h"

void midiproc( struct midistatus *midi, uint8_t byte, uint8_t channel )
{
	//Handle synthesizer reset
	if ( byte == 0xff ) midi->reset = 1;
	
	if ( byte & ( 1 << 7 ) ) //Handle status bytes
	{
		//Extract information from status byte
		midi->status = byte & 0x70;
		midi->dcnt = midi->dlim = 0;

		//Check data length for each MIDI command
		switch ( midi->status )
		{
			//Note on
			case 0x10:
				midi->dlim = 2;
				break;
				
			//Note off
			case 0x00:
				midi->dlim = 2;
				break;
			
			//Controller change
			case 0x30:
				midi->dlim = 2;
				break;
			
			//Program change
			case 0x40:
				midi->dlim = 1;
				break;
				
			//Pitch
			case 0x60:
				midi->dlim = 2;
				break;
			
			//Uknown command
			default:
				midi->dlim = 0;
				break;
				
		}
	}
	else if ( ( byte & 0x0f ) == channel ) //Handle data bytes
	{
		//Data byte
		midi->dbuf[midi->dcnt++] = byte;
		
		//Interpret command
		if ( midi->dcnt >= midi->dlim )
		{
			switch ( midi->status )
			{
				//Note on
				case 0x10:
					midi->note = midi->dbuf[0];
					midi->notevel = midi->dbuf[1];
					midi->noteon = 1;
					break;
					
				//Note off	
				case 0x00:
					if ( midi->note == midi->dbuf[0] )
						midi->noteon = 0;
					break;
					
				//Controller change
				case 0x30:
					midi->controllers.raw[midi->dbuf[0]] = midi->dbuf[1];
					break;
									
				//Program change
				case 0x40:
					midi->program = midi->dbuf[0];
					break;
					
				//Pitch
				case 0x60:
					midi->pitchbend = midi->dbuf[0] | ( midi->dbuf[1] << 7 );
					break;
					

				default:
					break;
			}
			
			midi->dcnt = 0;
		}
		
	}
}
