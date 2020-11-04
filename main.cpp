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

#include "ssd1306/ssd1306.h"



int main(void)
{
	uint8_t lightUp				= 1;
	uint8_t displayTwiAddress	= 0x3c;
	uint8_t	width				= 128;
	uint8_t height				= 64;
	ssd1306	oled;



	//	Enable output to LED port
	DDRB |= 0b00100000;

	// Blink LED to visualise start of program
	_delay_us(500000);
	PORTB ^= 0b00100000;
	_delay_us(200000);
	PORTB ^= 0b00100000;
	_delay_us(200000);
	_delay_us(490000);

	// Here goes the main code...
	switch(oled.initDisplay(displayTwiAddress, width, height)){

		case MEMORY_ALLOC_ERROR:	// if we're not able to allocate memory for the buffer, flash the LED 13 times fast
									oled.twi.blinkLED(0b00100000, 13, 0);
									break;

		case TWI_INITDISPLAY_OK:	// we got through initDisplay without errors
									oled.twi.blinkLED(0b00100000, 2, 1);
									break;

		default:					oled.twi.blinkLED(0b00100000, 20, 1);
									break;

	}





	//	Wait a bit, then light up the built-in LED if we got no errors
	if(lightUp){
		_delay_us(200000);
		PORTB |= 0b00100000;
	}

	return 1;
}


