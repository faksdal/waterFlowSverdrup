/*
 *	ssd1306.h
 *
 *	Created on: 31 Oct 2020
 *	Author: jole
 *
 *	According to the data sheet 'SSD1306 Application Note' p.5, the initialization sequence
 *	of the display is as follows: (page numbers relates to 'SSD1306 Advance Information')
 *		- set MUX ratio
 *			described on p.31 0xa8, 0x3f
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



class ssd1306 {
public:
	ssd1306();

	//virtual ~ssd1306();
	//ssd1306(const ssd1306 &other);
};

#endif /* ATMEGA328P_WATERFLOWSVERDRUP_SRC_SSD1306_SSD1306_H_ */
