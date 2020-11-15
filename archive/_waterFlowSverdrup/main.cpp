/*
 * main.c
 *
 *  Created on: 18 Oct 2020
 *      Author: jole
 */



#define F_CPU	16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "twi/twi.h"



int main(void)
{
	twiAtmega328P	twi;


	//
	// set bit 5 in PORTC to output. This is the built-in LED
	//
	DDRB |= 0b00100000;

	if(twi.init(400000)){
		//blinkLED(0b00100000, 3, 0);
		//_delay_us(300000);
		//return(-1);
	}

	if(twi.sendStart()){
		//blinkLED(0b00100000, 11, 0);
		//_delay_us(300000);
		//return(-1);
	}

	while((twi.mtMode(0x3c) >> 3) != 0b00000011){ // SLA_W_ACK
		blinkLED(0b00100000, 3, 0);
		_delay_us(300000);
		//return(-1);
	}
	twi.write(0x3c, 0, 0xa8);
	twi.write(0x3c, 0, 0xd3);
	twi.write(0x3c, 0, 0x40);
	twi.write(0x3c, 0, 0xa0);
	twi.write(0x3c, 0, 0xc0);
	twi.write(0x3c, 0, 0xda);
	twi.write(0x3c, 0, 0x81);
	twi.write(0x3c, 0, 0xa4);
	twi.write(0x3c, 0, 0xa6);
	twi.write(0x3c, 0, 0xd5);
	twi.write(0x3c, 0, 0x8d);
	twi.write(0x3c, 0, 0xaf);
	twi.write(0x3c, 0b01000000, 'a');
	twi.write(0x3c, 0b01000000, 'b');
	twi.write(0x3c, 0b01000000, 'c');
	twi.write(0x3c, 0b01000000, 'd');
	twi.stop();

	_delay_us(500000);

	//
	// light up if we're here ok
	PORTB |= 0b00100000;

	return 1;
}


