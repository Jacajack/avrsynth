# avrsynth
![The GPL License](https://img.shields.io/badge/license-GPL-blue.svg?style=flat-square)

### A small avr monofonic synthesier based on atmega32 microcontroller.

![image]( https://imgur.com/QG5PIJC.jpg )
*THT prototype*

#### Synthesier contains:
- LFO
- ASR envelope
- MIDI interpreter
- different wave shapes
- noise generator

#### Power
Synthesier needs 5 - 18V DC power supply. You can use:
- 2.5mm barrel jack power cord
- ordinary wires connected to a screw socket

#### Audio
Generating digital audio using interrupts and 8bit samples.
<br>Changing volume not only using potentiometer.
<br>Plays frequencies from 8 to 12543 Hz, more than 10 octaves!

#### USART
USART baud ratio is set to 9600 baud, 8 data and 1 stop bit.

#### Connectors
- 3.5mm stereo jack socket
- DIN-5 MIDI socket
- power barrel socket
- power screw socket
