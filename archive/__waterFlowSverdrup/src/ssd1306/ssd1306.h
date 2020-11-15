/*
 *	ssd1306.h
 *
 *	Created on: 31 Oct 2020
 *	Author: jole
 *
 *	According to the data sheet 'SSD1306 Application Note' p.5, the initialization
 *	sequence of the display is as follows: (page numbers relates to 'SSD1306 Advance Information')
 *		- set MUX ratio
 *		- set display offset
 *		- set display start line
 *		- set segment re-map
 *		- set COM output scan direction
 *		- set COM pins hardware configuration
 *		- set contrast control
 *		- disable entire display On, 0xa4
 *		- set normal display
 *		- set oscillator frequency
 *		- enable charge pump regulator
 *		- display On
 *
 */

#ifndef ATMEGA328P_WATERFLOWSVERDRUP_SRC_SSD1306_SSD1306_H_
#define ATMEGA328P_WATERFLOWSVERDRUP_SRC_SSD1306_SSD1306_H_

#include <ssd1306/ssd1306CommandSet.h>
#include "twi328P/twi328P.h"

#define DISPLAY_BUFFER_SIZE	128	//1136


class ssd1306 {

	volatile uint8_t* buffer;//[DISPLAY_BUFFER_SIZE];

	twi328P twi;

	uint8_t	slaveAddress, displayWidth, displayHeight;

public:
	ssd1306();

	uint8_t	initDisplay(uint8_t _slaveAddress, uint8_t displayWidth, uint8_t displayHeight);
	uint8_t getDisplayWidth(void) { return(displayWidth);}
	uint8_t getDisplayHeight(void) {return(displayHeight);}
	void	drawPixel(int16_t x, int16_t y, uint16_t color);
	void	clearDisplay(void);
	void	display(void);

};

#endif /* ATMEGA328P_WATERFLOWSVERDRUP_SRC_SSD1306_SSD1306_H_ */
