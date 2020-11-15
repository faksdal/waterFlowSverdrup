/*
 *	twi328P.cpp
 *
 *	Created on: 31 Oct 2020
 *	Author: jole
 */

#include "twi328P/twi328P.h"

#include <twi328P/twiErrorHandling.cpp>
#include <twi328P/initBus.cpp>
#include <twi328P/disableBus.cpp>
#include <twi328P/sendStart.cpp>
#include <twi328P/waitForTWINTFlag.cpp>
#include <twi328P/write.cpp>



//
//	constructor, for creating the object and do some preparations
//
twi328P::twi328P(void)
{
	//
	//	initialize the reStarts value, and sets mode to none
	//
	reStarts	= 0;
	twiBusMode	= none;

	//
	// set bit 5 in PORTC to output. This is the built-in LED
	//
	DDRB	=	0b00100000;
	//PORTB	|=	0b00100000;

	return;
}



//
//	Function:		twi328P::sendSlaW()
//
//	Description:	transmit a SLA+W onto the twi bus
//					checking the status register for appropriate value, blinking LED's as necessary
//
//
//
//	Arguments:		none
//
//	return value:	returns status register with prescaler bits masked out: TWSR & 0xf8
//
uint8_t twi328P::sendSlaW(uint8_t _slaveAddress)
{
	TWDR = (_slaveAddress << 1) | TW_WRITE;

	//
	// initiate transmit of SLA+W, and wait for the hardware to respond...
	//
	TWCR = (1 << TWINT) | (1 << TWEN);
	waitForTWINTFlag();

	//
	//	check the TWSR and blink the LED as needed...
	//
	return(checkStatusRegister(TWI_SLA_W_TRANSMITTED_ACK_RECEIVED));

	//return(TWI_SLA_W_TRANSMITTED_ACK_RECEIVED);
}
// END twi328P::sendSlaW()



//
//	Function:		twi328P::stop()
//
//	Description:	sends STOP condition to the twi bus
//
//	Arguments:		none
//
//	return value:	none
//
void twi328P::sendStop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
// END twi328P::stop()



/*
void twi328P::stop(void)
{
	//
	// send stop condition
	//
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}



uint8_t twi328P::twsrStatus(void)
{


					—————————————————————————————————————————————————————————————————
			TWSR	| TWS7  |  TWS6 | TWS5  | TWS4  | TWS3  |   -   | TWPS1 | TWPS0 |
			    	|   0   |   0   |   0   |   0   |   1   |   0   |   0   |   0   |
					—————————————————————————————————————————————————————————————————

			In order to read the status codes from the TWSR we need to mask out the
			prescaler bits TWPS1 and TWPS0, see data sheet p.200



	//
	// create the mask
	//
	//uint8_t mask		= ~(0b00000011);
	//uint8_t twStatus	= TWSR & mask;

	return(TWSR & 0xf8);
}


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
*/
