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
	uint8_t lightUp			= 1;
	uint8_t displayAddress	= 0x3c;

	ssd1306	display;

	PORTB ^= 0b00100000;
	_delay_us(500000);
	PORTB ^= 0b00100000;
	_delay_us(2500000);

	//uint8_t retVal = display.initDisplay(displayAddress, 128, 64);

	switch(display.initDisplay(displayAddress, 128, 64)){

		case TWI_OK:								for(int i = 0; i < 3; i++){
														PORTB ^= 0b00100000;
														_delay_us(500000);
														PORTB ^= 0b00100000;
														_delay_us(500000);
													}
													break;

		case TWI_START_TRANSMITTED:					for(int i = 0; i < (TWSR & 0xf8); i++){
														PORTB ^= 0b00100000;
														_delay_us(500000);
														PORTB ^= 0b00100000;
														_delay_us(500000);
													}
													break;
		case TWI_SLA_W_TRANSMITTED_ACK_RECEIVED:	for(int i = 0; i < (TWSR & 0xf8); i++){
														PORTB ^= 0b00100000;
														_delay_us(500000);
														PORTB ^= 0b00100000;
														_delay_us(500000);
													}
													break;
		case TWI_DATA_TRANSMITTED_ACK_RECEIVED:		for(int i = 0; i < (TWSR & 0xf8); i++){
														PORTB ^= 0b00100000;
														_delay_us(500000);
														PORTB ^= 0b00100000;
														_delay_us(500000);
													}
													break;

		default:									lightUp = 0;
													/*
													for(int i = 0; i < 11; i++){
														PORTB ^= 0b00100000;
														_delay_us(500000);
														PORTB ^= 0b00100000;
														_delay_us(500000);
													}
													*/
													break;
	}

	_delay_us(500000);


	/*
	for(int i =0;i<95;i++)
		display.drawPixel(i, 3, 1);
	display.display();
	*/

	//
	//	light up the built-in LED if we got no errors
	//
	if(lightUp)
		PORTB |= 0b00100000;




	return 1;
}


