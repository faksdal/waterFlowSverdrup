/*
 * twi328P.cpp
 *
 *  Created on: 31 Oct 2020
 *      Author: jole
 */

#include "twi328P/twi328P.h"

void blinkLED(uint8_t _PORT, uint8_t count, uint8_t blinkNormal)
{

	if(!blinkNormal){
		for(uint8_t i = 0; i < count; i++){
			PORTB |= _PORT;
			_delay_us(19000);
			PORTB &= ~_PORT;
			_delay_us(32000);
		}
	}
	else{
		for(uint8_t i = 0; i < count; i++){
			PORTB |= _PORT;
			_delay_us(125000);
			PORTB &= ~_PORT;
			_delay_us(255000);
		}
	}
}



//
//	constructor, for creating the object
//
twi328P::twi328P(void)
{
	reStarts	= 0;
	mode		= none;

	return;
}


//
//	init() initializes the i2c bus making it ready for transmission
//
//	init() sets TWBR according to the desired bus speed
//
//	init() returns 0 upon success, anything else indicates an error
//
uint8_t	twi328P::init(uint32_t _busSpeed	// pass the desired i2c bus speed as parameter
		                    )
{
	//
	//	standard mode:	100	kbit/s
	//	full speed:		400	kbit/s
	//	fast mode:		1	Mbit/s
	//	high speed:		3,2	Mbit/s
	//
	// if the user don't pass one of the supported speeds, return with an error
	//

	if(_busSpeed != 100000 && _busSpeed != 400000 && _busSpeed != 1000000 && _busSpeed != 3200000)
		return(1);

	//
	//	Formula for Bit Rate Generator Unit, see p.180 in data sheet
	//
	//	Here we calculate TWBR based on the desired i2c bus speed and
	//	clock frequency of the computer we use (F_CPU)
	//
	TWBR = ((F_CPU / _busSpeed - 16) / (2));


	//
	//	make SDA and SCL inputs by setting bit 4 and 5 in the
	//	data direction register of port C (DDRC) to 1
	//
	//      	-------------------------------------------------------------------------
	//	DDRC 	|   -    |  DDC6  |  DDC5  |  DDC4  |  DDC3  |  DDC2  |  DDC1  |  DDC0  |
	//      	-------------------------------------------------------------------------
	//      	-------------------------------------------------------------------------
	//      	|   x    |   x    |   1    |   1    |   x    |   x    |   x    |   x    |
	//      	-------------------------------------------------------------------------
	//
	//
	DDRC |= (3 << DDC4);

	//
	//	disable internal pull-ups on SDA and SCL
	//      	-----------------------------------------------------------------------------------------
	//	PORTC	|    -     |  PORTC6  |  PORTC5  |  PORTC4  |  PORTC3  |  PORTC2  |  PORTC1  |  PORTC0  |
	//      	-----------------------------------------------------------------------------------------
	//      	-----------------------------------------------------------------------------------------
	//      	|    x     |    x     |    0     |    0     |    x     |    x     |    x     |    x     |
	//      	-----------------------------------------------------------------------------------------
	//
	//
	//
	PORTC	|= ~(3 << DDC4);

	//
	//	Clear the prescaler bits in the TWSR, setting the prescaler to 1
	//	See p.200 in data sheet
	//
	//	The prescaler bits are the two LSB in the TWCR, TWPS1 and TWPS0
	//
	//			-----------------------------------------------------------------
	//	TWSR	| TWS7  |  TWS6 | TWS5  | TWS4  | TWS3  |   -   | TWPS1 | TWPS0 |
	//			|   x   |   x   |   x   |   x   |   x   |   x   |   0   |   0   |
	//			-----------------------------------------------------------------
	//
	TWSR &= ~(0b00000011);

	//	enable twi module, acks, and twi interrupt
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA);

	return(0);
}



void twi328P::disable(void)
{
  // disable twi module, acks, and twi interrupt
  TWCR &= ~(1 << (TWEN) | 1 << (TWIE) | 1 << (TWEA));
}



//
//	start() sends a STASRT condition to the bus, and responds to the feedback from
//	TWI hardware
//
//	It returns a non-zero value upon error
//
uint8_t twi328P::sendStart(void)
{
	uint8_t	retFlag = 0b00000000;


	//
	//	A START condition is sent by writing the following value to TWCR:
	//
	//			-----------------------------------------------------------------
	//	TWCR	| TWINT |  TWEA | TWSTA | TWSTO | TWWC  | TWEN  |   -   | TWIE  |
	//	    	|   1   |   x   |   1   |   0   |   x   |   1   |   0   |   x   |
	//			-----------------------------------------------------------------
	//
	//	TWINT must be written to one to clear the TWINT flag.
	//	TWSTA must be written to one to transmit a START condition.
	//	TWEN must be set to enable the 2-wire serial interface.
	//
	//	The TWI will then test the 2-wire serial bus and generate a START condition
	//	as soon as the bus becomes free.
	//

	//
	// send start condition
	//
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	//
	//	Now we wait for the hardware to:
	//		- set the TWINT flag in TWCR
	//		- update the TWSR with a status code
	//

	//
	//			-----------------------------------------------------------------
	//	TWCR	| TWINT |  TWEA | TWSTA | TWSTO | TWWC  | TWEN  |   -   | TWIE  |
	//	    	|   1   |   x   |   1   |   0   |   x   |   1   |   0   |   x   |
	//			-----------------------------------------------------------------
	//
	// Wait for the TWINT flag in TWCR to be set (e.g. write it to 0). This indicates that the
	// START condition is sent
	//
	reStarts = MAX_RESTARTS;
	while( ((TWCR & (1 << TWINT)) != 0) && reStarts != 0){
		reStarts--;
	}

	//
	// return an error if we strike out
	//
	if(reStarts <= 0)
		retFlag |= ERROR_RESTART_MAX;

	//
	// Now we need to examine the status of the TWSR
	//
	reStarts = MAX_RESTARTS;
	while (  ((TWSR & (0xf8)) != 0x08) && reStarts != 0 ){
		reStarts--;
	}

	retFlag |= 0x08;

	//
	// return an error if we strike out
	//
	if(reStarts <= 0)
		retFlag |= ERROR_RESTART_MAX;

	//
	// return 0 if we're ok
	//
	return(retFlag);

}



uint8_t	twi328P::mtMode(uint8_t slaveAddress)
{
	uint8_t	retFlag = 0b00000000;


	//
	//			-------------------------------------------------------------------------
	//	TWDR	|  TWD7  |  TWD6  |  TWD5  |  TWD4  |  TWD3  |  TWD2  |  TWD1  |  TWD0  |
	//	    	|  0/1   |  0/1   |  0/1   |  0/1   |  0/1   |  0/1   |  0/1   |  0/1   |
	//			-------------------------------------------------------------------------
	//
	// Write SLA+W to TWDR to enter Master Transmitter mode
	//
	// For the slave to accept a W, the last bit in the TWDR, TWD0, must be
	// set to 0, this indicates that we want to write to the slave
	//
	// keep in mind the slave address is only seven bits, so in order to
	// make it right, we need to shift the address one bit to the left, thus
	// making space for the R/W-bit
	//
	TWDR = (slaveAddress << 1) | TW_WRITE;

	//
	// load TWCR with appropriate values to continue the transfer
	//
	TWCR = (1 << TWINT) | (1 << TWEN);

	//
	//	wait for the TWINT flag to be set by TWI hardware
	//
	reStarts = MAX_RESTARTS;
	while( ((TWCR & (1 << TWINT)) != 0) && reStarts != 0){
		reStarts--;
	}

	//
	// return an error if we strike out
	//
	if(reStarts <= 0)
		retFlag |= ERROR_RESTART_MAX;

	//
	//	now we need to check the response from TWI hardware by reading TWSR
	//
	//	for a transmitted SLA+W, it should read:
	//		0x18 for successfull transmission of SLA+W, ACK has been received
	//		0x20 for successfull transmission of SLA+W, NOT ACK has been received
	//			-----------------------------------------------------------------
	//	TWSR	| TWS7  |  TWS6 | TWS5  | TWS4  | TWS3  |   -   | TWPS1 | TWPS0 |
	//	0x18	|   0   |   0   |   0   |   1   |   1   |   0   |   0   |   0   |
	//			-----------------------------------------------------------------
	//
	//			-----------------------------------------------------------------
	//	TWSR	| TWS7  |  TWS6 | TWS5  | TWS4  | TWS3  |   -   | TWPS1 | TWPS0 |
	//	0x20	|   0   |   0   |   1   |   0   |   0   |   0   |   0   |   0   |
	//			-----------------------------------------------------------------
	//

	//
	// 	waiting for TWSR to be set
	//
	reStarts = MAX_RESTARTS;
	while (  ((TWSR & (0xf8)) != 0x18) && ((TWSR & (0xf8)) != 0x20) && reStarts != 0 ){
		reStarts--;
	}

	//
	// return an error if we strike out
	//
	if(reStarts <= 0)
		retFlag |= ERROR_RESTART_MAX;

	//	blink for debugging...
	if(((TWSR & (0xf8)) != 0x18)){
		//blinkLED(0b00100000, 18, 1);
		//_delay_us(300000);
		retFlag |= 0x18;
	}
	else if(((TWSR & (0xf8)) != 0x20)){
		//blinkLED(0b00100000, 20, 1);
		//_delay_us(300000);
		retFlag |= 0x20;
	}

	return(retFlag);
}



//
//
//
uint8_t twi328P::write(uint8_t	_slaveAddress,	//
		                     uint8_t	address,		//
		                     uint8_t	data			//
							 //i2cMode	_mode			//
							 )
{



	//
	//After slave address is sent, we need to check for acknowledge
	//
	// Waiting for twi hardware to set TWINT flag by writing a 0
	//
	/*while((TWCR & (1 << TWINT)) == 0){
		//blinkLED(0b00100000, 3, 1);
		//_delay_us(300000);
	}*/

	//
	// We also need to check the status register to see we got an ACK
	//
	/*while( ((TWSR & (0xf8)) != 0x18) && ((TWSR & (0xf8)) != 0x28) ){
		//blinkLED(0b00100000, 7, 1);
		//_delay_us(300000);
	}*/


	//
	// write control to TWDR
	//
	/*
	TWDR = control;

	TWCR = (1 << TWINT) | (1 << TWEN);

	while((TWCR & (1 << TWINT)) == 0){
		//blinkLED(0b00100000, 2, 1);
		//_delay_us(300000);
	}

	while((TWSR & (0xf8)) != 0x28){
		//blinkLED(0b00100000, 4, 1);
		//_delay_us(300000);
	}
	*/

	TWDR = address;

	TWCR = (1 << TWINT) | (1 << TWEN);

	while(!(TWCR & (1 << TWINT))){
		//blinkLED(0b00100000, 3, 0);
		//_delay_us(300000);
	}
	//
	// write data to TWDR
	//
	TWDR = data;

	TWCR = (1 << TWINT) | (1 << TWEN);

	while(!(TWCR & (1 << TWINT))){
		//blinkLED(0b00100000, 3, 0);
		//_delay_us(300000);
	}


	while((TWSR & (0xf8)) != 0x28){
		blinkLED(0b00100000, 3, 1);
		_delay_us(300000);
	}

	//this->stop();

	return(1);
}



void twi328P::stop(void)
{
	//
	// send stop condition
	//
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}



uint8_t twi328P::twsrStatus(void)
{
	/*

					—————————————————————————————————————————————————————————————————
			TWSR	| TWS7  |  TWS6 | TWS5  | TWS4  | TWS3  |   -   | TWPS1 | TWPS0 |
			    	|   0   |   0   |   0   |   0   |   1   |   0   |   0   |   0   |
					—————————————————————————————————————————————————————————————————

			In order to read the status codes from the TWSR we need to mask out the
			prescaler bits TWPS1 and TWPS0, see data sheet p.200

	 */

	//
	// create the mask
	//
	//uint8_t mask		= ~(0b00000011);
	//uint8_t twStatus	= TWSR & mask;

	return(TWSR & 0xf8);
}

/*
uint8_t i2cPutByte(uint8_t byte)
{
	//
	// put the byte to send into the data register
	//
	TWDR = byte;

	TWCR = 1 << TWINT | 1 << TWEN;

	while((TWCR & _BV(TWINT)) == 0){
	}

	switch(TW_STATUS){
		case TW_MT_SLA_ACK:
		case TW_MT_DATA_ACK:
		case TW_MR_SLA_ACK:
		case TW_MR_DATA_ACK:	return(0);
								break;
		case TW_BUS_ERROR:
		case TW_MT_SLA_NACK:
		case TW_MT_DATA_NACK:
		case TW_MT_ARB_LOST:
		case TW_MR_SLA_NACK:
		case TW_MR_DATA_NACK:	return(1);
								break;
		default:				return(2);
								break;
	}
}


*/


