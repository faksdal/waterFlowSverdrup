/*
 * ssd1306.cpp
 *
 *  Created on: 31 Oct 2020
 *      Author: jole
 */

#include <stdlib.h>
#include <string.h>
#include "ssd1306/ssd1306.h"




ssd1306::ssd1306()
{
	slaveAddress = 0b00000000;
}



//
//	Function:		ssd1306::initDisplay()
//
//	Description:	calls twi.initBus(), this initialises i2c/twi bus, making it ready for transmission
//					calls twi.sendStart(), this transmits the START condition onto the twi bus
//					calls twi.sendSlaW(), transmitting the SLA+W to the twi bus
//					calls initialisation sequence for the display
//					calls twi.sendStop() to transmit a STOP onto the bus
//
//	arguments:		none
//
//	return value:	ERROR_BUS_SPEED_OUT_OF_RANGE if the desired bus speed is not within i2c specs
//
//
uint8_t ssd1306::initDisplay(uint8_t _slaveAddress, uint8_t _displayWidth, uint8_t _displayHeight)
{
	//
	//	initialise the twi bus
	//
	switch(twi.initBus(400000UL)){
			case TWI_ERROR_BUS_SPEED_OUT_OF_RANGE:	twi.blinkLED(0b00100000, 2, 1);
													_delay_us(500000);
													return(TWI_ERROR_BUS_SPEED_OUT_OF_RANGE);
													break;

			case TWI_BUS_INITIALISE_SENT:			twi.blinkLED(0b00100000, 3, 1);
													_delay_us(500000);
													//twi.twiBusMode = twi.busInitialiseSent;
													break;

			default:								break;
	}



	//
	//	set display width and height
	//
	displayWidth	=	_displayWidth;
	displayHeight	=	_displayHeight;

	//
	//	allocate memory for display buffer
	//	returning an error if we fail...
	//
	//buffer = (uint8_t*)malloc((displayWidth * ((displayHeight + 7) / 8)));
	//buffer =  malloc(256);
	//if(!buffer)
	//	return(-1);	//	TODO: #define error code for memory allocation fail

	//
	//	clear the buffer...
	//
	//memset((uint8_t*) buffer, 0, (displayWidth * ((displayHeight + 7) / 8)));
	//clearDisplay();
	//for(uint16_t i = 0; i <= DISPLAY_BUFFER_SIZE; i++)
	//	buffer[i] = 0;



	//
	//	transmit START condition onto the bus
	//
	switch(twi.sendStart()){
		case TWI_START_TRANSMITTED:		// if we got TWI_START_TRANSMITTED, we know we're ok
										//twi.checkStatusRegister(TWI_START_TRANSMITTED);
										twi.blinkLED(0b00100000, 4, 1);
										_delay_us(500000);
										break;

		case TWI_REPEATED_START_TRANSMITTED:		// if we got TWI_START_TRANSMITTED, we know we're ok
												//twi.checkStatusRegister(TWI_START_TRANSMITTED);
												twi.blinkLED(0b00100000, 5, 1);
												_delay_us(500000);
												break;

		default:						// if we don't get TWI_START_TRANSMITTED we know something is wrong
										// so we blink the error sign
										//twi.twiBusError();
										// and return unspecified error
										twi.blinkLED(0b00100000, 6, 1);
										_delay_us(500000);
										for(int c = 0; c < (TWSR & 0xf8); c++){
											twi.blinkLED(0b00100000, 1, 1);
											_delay_us(500000);
										}
										return(-1);
										break;
	}



	//
	//	transmit SLA+W onto the bus
	//
	slaveAddress = _slaveAddress;
	switch(twi.sendSlaW(slaveAddress)){
		case TWI_SLA_W_TRANSMITTED_ACK_RECEIVED:	twi.blinkLED(0b00100000, 7, 1);
													_delay_us(500000);
													break;

		default:									twi.blinkLED(0b00100000, 8, 1);
													_delay_us(500000);
													return(-1);
													break;
	}



	//
	//	if we're ok this far, we can start sending the initialisation sequence to the ssd1306 display
	//
	//	this is done by sending a series of commands to the display, following a certain pattern
	//
	//	| Co |D/C#| 0/1 | 0/1 | 0/1 | 0/1 | 0/1 | 0/1 | ACK | Co |D/C#| 0/1 | 0/1 | 0/1 | 0/1 | 0/1 | 0/1 | 0/1 | 0/1 | ACK |
	//
	//		Co:		Continuation bit, set to logic zero. See p.20
	//		D/C#:	Data or Command, set to logic 0 for command, 1 for data
	//		0/1:	The actual data/command bits
	//		ACK:	ACK generated after receiving each command or data byte
	//
	//
	//		set MUX ratio, command 0xa8, see p.31
			twi.write(0, CMD_SET_MUX_RATIO);
			twi.write(1, MUX_RATIO_RESET);

	//		set display offset, command 0xd3, see p.31
			twi.write(0, CMD_SET_DISPLAY_OFFSET);
			twi.write(1, DISPLAY_OFFSET_RESET);

	//		set display start line, command 0x40, see p.31
	//
			twi.write(0, CMD_SET_DISPLAY_START_LINE);

	//		set segment re-map, command 0xa0, 0xa1, see p.31
	//		when set to 0b10100000 (0xa0), column address 0 is mapped to SEG0 (reset)
	//		when set to 0b10100001 (0xa1), column address 127 is mapped to SEG0 (flipped)
			twi.write(0, SEGMENT_MAP_NORMAL);

	//		set COM output scan direction, command 0xc0, 0xc8, see p.31
			twi.write(0, COM_OUTPUT_SCAN_NORMAL);

	//		set COM pins hardware configuration, command 0xda, see p.31
			twi.write(0, CMD_SET_COM_PINS_HWCONF);
			twi.write(1, COM_PINS_HWCONF);

	//		set contrast control, command 0x81, see p.28
			twi.write(0, CMD_SET_CONTRAST_CONTROL);
			twi.write(1, CONTRAST_RESET);

	//		disable entire display On, command 0xa4, see p.28
			twi.write(0, DISPLAY_RAM_CONTENT);
			//twi.write(0, IGNORE_RAM_CONTENT);

	//		set normal display, command 0xa6, see p. 28
			twi.write(0, SET_NORMAL_DISPLAY);
			//twi.write(0, SET_INVERSE_DISPLAY);

	//		set oscillator frequency, command 0xd5, see p.32
			twi.write(0, SET_CLOCK_DIVIDE_RATIO_OSC_FREQ);
			twi.write(1, CLOCK_DIVIDE_RATIO_OSC_FREQ_RESET);

	//		the display memory should be cleared before the charge pump command gets executed
	//		memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));

	//		enable charge pump regulator, command 0x8d, see p.3 in 'SSD1306 Application Note'
			twi.write(0, SET_CHARGE_PUMPE);
			twi.write(1, CHARGE_PUMPE_ENABLE);
			twi.write(1, CHARGE_PUMPE_DISPLAY_ON);

	//		display On, command 0xaf, see p.28
			twi.write(0, SET_DISPLAY_ON);

	return(TWI_OK);
}
//	END ssd1306::initDisplay()




//
//	Function:		ssd1306::drawPixel()
//
//	Description:	draw one pixel at location x, y in display ram buffer
//
//	arguments:		int16_t x:		x-location of pixel to be drawn
//					int16_t y:		y-location of pixel to be drawn
//					uint16_t color:	color of pixel to be drawn. The SSD1306 doesn't support colors. We can only draw them in WHITE or BLACK
//
//	return value:	none
//
//
void ssd1306::drawPixel(int16_t x, int16_t y, uint16_t color) {


	if ((x >= 0) && (x <displayWidth) && (y >= 0) && (y < displayHeight)) {

		// Pixel is in-bounds. Rotate coordinates if needed.
	/*
		switch (getRotation()) {
			case 1:		ssd1306_swap(x, y);
						x = WIDTH - x - 1;
						break;

			case 2:		x = WIDTH - x - 1;
						y = HEIGHT - y - 1;
						break;

			case 3:		ssd1306_swap(x, y);
						y = HEIGHT - y - 1;
						break;
    	}
    */
		switch (color){
			case 1 /*SSD1306_WHITE*/:	buffer[x + (y / 8) * displayWidth] |= (1 << (y & 7));
										break;
			case 0 /*SSD1306_BLACK*/:	buffer[x + (y / 8) * displayWidth] &= ~(1 << (y & 7));
										break;
			case 2 /*SSD1306_INVERSE*/:	buffer[x + (y / 8) * displayWidth] ^= (1 << (y & 7));
										break;
		}
	}
}
// END ssd1306::drawPixel()



//
//	Function:		ssd1306::clearDisplay()
//
//	Description:	clear display memory buffer, writing it to all 0's
//
//	arguments:		none
//
//	return value:	none
//
//
void ssd1306::clearDisplay(void)
{
	//
	//	clear the buffer...
	//
	//memset((uint8_t*) buffer, 0, (displayWidth * ((displayHeight + 7) / 8)));

	for(uint16_t i = 0; i < DISPLAY_BUFFER_SIZE; i++)
		buffer[i] = 0;
}
// END ssd1306::clearDisplay(



//
//	Function:		ssd1306::display()
//
//	Description:	refresh display, putting content of RAM onto the display
//
//	arguments:		none
//
//	return value:	none
//
//
void ssd1306::display(void)
{
	// steps for updating the display, writing memory content to the device

	// - set pageadddress SSD1306_PAGEADDR		0x22
	// - set page start						0x00
	// - set page end address					0xff
		twi.write(0, SET_PAGE_ADDRESS);
		twi.write(1, START_PAGE_RESET);
		twi.write(1, END_PAGE_RESET);

	// - set column address SSD1306_COLUMNADDR	0x21
	// - column start address					0
	// - column end address					(WIDTH - 1)
		twi.write(0, SET_COLUMN_ADDRESS);
		twi.write(1, START_COLUMN_RESET);
		twi.write(1, END_COLUMN_RESET);

	// - start i2c transmission, sending slave address as parameter, beginTransmission
	// - write the value 0x40
		twi.write(0, SET_COLUMN_ADDRESS);

	// - write the scren buffer, one byte at a time until done. There is a check here, if we exceed WIRE_MAX, the bus shuts down
		//uint16_t count = displayWidth * ((displayHeight + 7) / 8);
		uint16_t count = DISPLAY_BUFFER_SIZE;
		volatile uint8_t *ptr = buffer;
		twi.sendStart();
		twi.sendSlaW(slaveAddress);
		twi.write(1, 0x40);
		while(count--)
			twi.write(*ptr++);


	//   and restarts. This is probably some hardware thing. WIRE_MAX equals 32 in this piece of code
	//   uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
	//    uint8_t *ptr = buffer;
	//    if (wire) { // I2C
	//      wire->beginTransmission(i2caddr);
	//      WIRE_WRITE((uint8_t)0x40);
	//      uint8_t bytesOut = 1;
	//      while (count--) {
	//        if (bytesOut >= WIRE_MAX) {
	//          wire->endTransmission();
	//          wire->beginTransmission(i2caddr);
	//          WIRE_WRITE((uint8_t)0x40);
	//          bytesOut = 1;
	//        }
	//        WIRE_WRITE(*ptr++);
	//        bytesOut++;
	//    }
	//    wire->endTransmission();
	//
	//    } else { // SPI
	//      SSD1306_MODE_DATA
	//      while (count--)
	//        SPIwrite(*ptr++);
	//    }

	/*
	//uint8_t getDisplayWidth() = 128;
	//uint8_t getDisplayHeight() = 64;
	//uint8_t *buffer;


	uint8_t *ptr	= buffer;
	//uint16_t count	= WIDTH * ((HEIGHT + 7) / 8);
	//for(int i = 0; i < 1024; i++)
	//	buffer[i] |= (1 << (i/16 & 7));

	//	set page address
	twi.write(0, 0x22);
	twi.write(1, 0);
	twi.write(1, 0xff);

	//	set column address
	twi.write(0, 0x21);
	twi.write(1, 0);
	twi.write(1, 63);

	//	write out content, counting to zero according to formula uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
	 */


}
// END ssd1306::display()


/*
 // REFRESH DISPLAY ---------------------------------------------------------

//	!
//	@brief	Push data currently in RAM to SSD1306 display.
//	@return	None (void).
//	@note	Drawing operations are not visible until this function is
//			called. Call after each graphics command, or after a whole set
//			of graphics commands, as best needed by one's own application.

void Adafruit_SSD1306::display(void) {
  TRANSACTION_START
  static const uint8_t PROGMEM dlist1[] = {
    SSD1306_PAGEADDR,
    0,                         // Page start address
    0xFF,                      // Page end (not really, but works here)
    SSD1306_COLUMNADDR,
    0 };                       // Column start address
  ssd1306_commandList(dlist1, sizeof(dlist1));
  ssd1306_command1(WIDTH - 1); // Column end address

#if defined(ESP8266)
  // ESP8266 needs a periodic yield() call to avoid watchdog reset.
  // With the limited size of SSD1306 displays, and the fast bitrate
  // being used (1 MHz or more), I think one yield() immediately before
  // a screen write and one immediately after should cover it.  But if
  // not, if this becomes a problem, yields() might be added in the
  // 32-byte transfer condition below.
  yield();
#endif
  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr   = buffer;
  if(wire) { // I2C
    wire->beginTransmission(i2caddr);
    WIRE_WRITE((uint8_t)0x40);
    uint8_t bytesOut = 1;
    while(count--) {
      if(bytesOut >= 32) { // Wire uses 32-byte transfer blocks max
        wire->endTransmission();
        wire->beginTransmission(i2caddr);
        WIRE_WRITE((uint8_t)0x40);
        bytesOut = 1;
      }
      WIRE_WRITE(*ptr++);
      bytesOut++;
    }
    wire->endTransmission();
  } else { // SPI
    SSD1306_MODE_DATA
    while(count--) SPIwrite(*ptr++);
  }
  TRANSACTION_END
#if defined(ESP8266)
  yield();
#endif
}

  */
