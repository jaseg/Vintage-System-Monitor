/* 	Vintage multimeter system monitor
	Copyright (C) 2011 by Sebastian Götte <s@jaseg.de>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../requests.h" //TODO bad style.
#include "usbdrv.h"

#define DEBOUNCE_TIME	200

uint8_t old_input_state; //87654321
						 // 1 - button
						 // 2 - position sensor
uint8_t switch_wait_time;

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t* rq = (usbRequest_t*) data;
	switch(rq->bRequest){
		case MON_SET:
			if(rq->wValue.bytes[0]){
				OCR1AH = rq->wIndex.bytes[1];
				OCR1AL = rq->wIndex.bytes[0];
			}else{
				OCR1BH = rq->wIndex.bytes[1];
				OCR1BL = rq->wIndex.bytes[0];
			}
			break;
	}
	return USB_NO_MSG; //Is this correct? FIXME
}

int main(void)
{
	//Device initialization
	PORTB |= 0x06; //PB1, PB2 (OC1A, OC1B)
	//Timer 1: Fast PWM, top ICR1
	TCCR1A = 0xA2;
	TCCR1B = 0x19;
	ICR1 = 0xFFFF;
	OCR1A = 0x0000;
	OCR1B = 0x0000;
	//USB Initialization
	usbInit();
	sei();
	//Infinitely eternal main loop.
	for(;;){
		usbPoll();
		_delay_ms(1);
	}
	return 0;
}

