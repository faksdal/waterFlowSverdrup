/*
 * main.c
 *
 *  Created on: 18 Oct 2020
 *      Author: jole
 */



#define F_CPU	16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#include "yfs401/yfs401.h"
#include "ssd1306/ssd1306.h"



volatile	uint64_t	reset			= 0;
volatile	long double	waterTotal		= 0;
volatile	float		waterLast		= 0;
static		double		pulsesPerLiter	= 5026.0;
			ssd1306		oled;
			uint8_t		text[64];
			uint8_t		liter[16];



ISR(INT1_vect)
{
	//pulses++;
	waterTotal	+= (1 / pulsesPerLiter);
	waterLast	+= (1 / pulsesPerLiter);
}



ISR(INT0_vect)
{
	waterLast	= 0;
	reset++;
}





int main(void)
{
	uint8_t		lightUp				= 1;
	uint8_t		displayTwiAddress	= 0x3c;
	uint8_t		width				= 128;
	uint8_t		height				= 64;

	//yfs401		flowMeter;



	//	Enable output to LED port
	DDRB |= 0b00100000;

	// Blink LED to visualise start of program
	oled.twi.blinkLED(0b00100000, 13, 0);
	_delay_us(500000);


	// Here goes the main code...

	// initialise the display, and check the feedback...
	switch(oled.initDisplay(displayTwiAddress, width, height)){

		case 1:					oled.twi.blinkLED(0b00100000, 3, 1);
								lightUp = 0;
								break;
		case 2:					oled.twi.blinkLED(0b00100000, 4, 1);
								lightUp = 0;
								break;
		case 3:					oled.twi.blinkLED(0b00100000, 5, 1);
								lightUp = 0;
								break;
		case 4:					oled.twi.blinkLED(0b00100000, 6, 1);
								lightUp = 0;
								break;
		case 5:					oled.twi.blinkLED(0b00100000, 7, 1);
								lightUp = 0;
								break;

		case MEMORY_ALLOC_ERROR:	// if we're not able to allocate memory for the buffer, flash the LED 13 times fast
									oled.twi.blinkLED(0b00100000, 13, 0);
									lightUp = 0;
									break;

		case TWI_INIT_OK:			// we got through initDisplay without errors
									oled.twi.blinkLED(0b00100000, 3, 0);
									break;



		default:					oled.twi.blinkLED(0b00100000, 20, 1);
									lightUp = 0;
									break;

	}

	DDRC	= 0xff;
	DDRD	= 0xe0;

	// enable global interrupts - see p.20
	SREG |= 0b10000000;

	// enable interrupt INT0 and INT1 on rising edge - see p.80
	EICRA	|=	0b00001010;

	// enable external interrupt INT0 and INT1 - see p.81
	EIMSK	|=	0b00000011;

	// enable PCINT0 interrupt see p.83
	//PCMSK0	|= (1 << PCINT0);
	//PCIFR	|= (1 << PCIF0);

	// see p.82
	//PCICR	|= (1 << PCIE0);


	/* Enable change of Interrupt Vectors */
	//MCUCR |= (1<<IVCE);
	/* Move interrupts to Boot Flash section */
	//MCUCR |= (1<<IVSEL);

	sei();

	PORTB = 0b00100000;

	while(1){
		/*
		dtostrf(pulses, 11, 1, (char*)liter);
		sprintf((char*)text, "Pulses:    %s", liter);
		oled.print(0, 0, text, strlen((char*)text));
		*/

		dtostrf((waterTotal), 9, 1, (char*)liter);
		sprintf((char*)text, "Total water: %s", liter);
		oled.print(0, 2, text, strlen((char*)text));

		dtostrf((waterLast), 9, 1, (char*)liter);
		sprintf((char*)text, "Last water:  %s", liter);
		oled.print(0, 4, text, strlen((char*)text));

		sprintf((char*)text, "Sverdrup Research");
		oled.print(4, 0, text, strlen((char*)text));

		/*
		if(reset > 0){
			dtostrf((reset), 4, 0, (char*)liter);
			sprintf((char*)text, "Reset:  %s times :-)", liter);
			oled.print(0, 5, text, strlen((char*)text));
		}
		*/

		oled.display();

	}


	return 1;
}




