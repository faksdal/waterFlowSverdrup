/*
 *	twi328P.cpp
 *
 *	Created on: 31 Oct 2020
 *	Author: jole
 */

#include "twi328P/twi328P.h"




//
//	Function:		twi328P::twi328P()
//
//	Description:	constructor for creating the object
//
//
//	Arguments:		none
//
//	return value:	none
//
twi328P::twi328P(void)
{

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
	return 1;
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

}
// END twi328P::stop()



//
//	Function:		twi328P::initBus()
//
//	Description:	initializes the i2c/twi bus, making it ready for transmission
//					sets TWBR according to the desired bus speed based on formula on p.222 in the data sheet
//					it also sets the prescaler bits in the TWSR, p.241 in the data sheet
//					This should be called only once by the application
//
//	arguments:		none
//
//	return value:	none
//
void	twi328P::initBus(uint32_t _busSpeed)
{
	//
	//	Sets the bus speed required by the application
	//
	busSpeed = _busSpeed;

	//
	//	Making sure the power reduction register (PRR), bit 7 is set to zero
	//	This is the power reduction for TWI and shuts down the bus if set to one
	//	See p.54 in the data sheet
	//
	//      	-------------------------------------------------------------------------
	//	PRR 	| PRTWI | PRTIM2 | PRTIM0 |   -   | PRTIM1 |  PRSPI  | PRUSART0 | PRADC |
	//      	-------------------------------------------------------------------------
	//      	|   0   |   x    |   x    |   x   |   x    |    x    |    x     |  x    |
	//      	-------------------------------------------------------------------------
	//
	//
	PRR &= ~(1 << PRTWI);

	//
	//	Formula for Bit Rate Generator Unit, see p.221 in the data sheet
	//
	//	Here we calculate TWBR based on the desired i2c bus speed and
	//	clock frequency of the computer we use (F_CPU)
	//
	TWBR = ((F_CPU / busSpeed - 16) / (2));

	//
	//	Make SDA and SCL inputs by setting bit 4 and 5 in the
	//	data direction register of port C (DDRC) to one
	//	See p.100 in the data sheet
	//
	//      	-------------------------------------------------------------------------
	//	DDRC 	|   -    |  DDC6  |  DDC5  |  DDC4  |  DDC3  |  DDC2  |  DDC1  |  DDC0  |
	//      	-------------------------------------------------------------------------
	//      	|   x    |   x    |   1    |   1    |   x    |   x    |   x    |   x    |
	//      	-------------------------------------------------------------------------
	//
	//
	DDRC |= (3 << DDC4);

	//
	//	Disable internal pull-ups on SDA and SCL
	//      	-----------------------------------------------------------------------------------------
	//	PORTC	|    -     |  PORTC6  |  PORTC5  |  PORTC4  |  PORTC3  |  PORTC2  |  PORTC1  |  PORTC0  |
	//      	-----------------------------------------------------------------------------------------
	//      	|    x     |    x     |    0     |    0     |    x     |    x     |    x     |    x     |
	//      	-----------------------------------------------------------------------------------------
	//
	//
	//
	PORTC	|= ~(3 << DDC4);

	//
	//	Clear the prescaler bits in the TWSR, setting the prescaler to 1
	//	See p.240 in data sheet
	//
	//	The prescaler bits are the two LSB in the TWCR, TWPS1 and TWPS0
	//
	//			-----------------------------------------------------------------
	//	TWSR	| TWS7  |  TWS6 | TWS5  | TWS4  | TWS3  |   -   | TWPS1 | TWPS0 |
	//			|   x   |   x   |   x   |   x   |   x   |   x   |   0   |   0   |
	//			-----------------------------------------------------------------
	//
	TWSR &= ~(0b00000011);

	//
	//	Enable acknowledgement
	//	Enable twi module
	//	Disable interrupts
	//
	TWCR = (1 << TWEA) | (1 << TWEN) | ~(1 << TWIE);


	//twiBusMode = busInitialiseSent;

	//return(TWI_BUS_INITIALISE_SENT);

}
//	END twi328P::initBus()




//
//	twi328P::disableBus()
//
//	disables the twi bus
//
void twi328P::disableBus(void)
{

}

//	END twi328P::disableBus()






//
//	Function:		twi328P::sendStart()
//
//	Description:	transmit a START condition onto the twi bus
//					it keeps track of how many times we loop waiting for the twi hardware
//					to set flag and update the status register
//					it'll blink the LED to show error or ok
//
//	Arguments:		none
//
//	return value:	returns status register with prescaler bits masked out: TWSR & 0xf8
//
uint8_t twi328P::sendStart(void)
{
	return 1;
}




//
//	Function:		twi328P::waitForTWINTFlag()
//
//	Description:	wait for the TWINT flag to set after writing commands to the twi bus
//
//	Arguments:		none
//
//	return value:	none
//
void twi328P::waitForTWINTFlag(void) {
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
	// Wait for the TWINT flag in TWCR to be set (e.g. write it to 0). This indicates successfull transmission
	//
	while (!(TWCR & (1 << TWINT))) {
	}
}
// END twi328P::waitForTWINTFlag()





//
//	Function:		twi328P::write()
//
//	Description:
//
//
//
//
//	Arguments:		uint8_t _slaveAddress	- address of slave device on twi bus
//					uint8_t mode			- specifies whether we want to write a command or data
//					uint8_t data			- command or data to be written
//
//	return value:	returns status register with prescaler bits masked out: TWSR & 0xf8
//
uint8_t twi328P::write(uint8_t _slaveAddress, uint8_t mode, uint8_t data)
{
	return 1;
}
// END twi328P::write()



//
//	Function:		twi328P::write()
//
//	Description:
//
//
//
//
//	Arguments:
//
//
//
//	return value:
//
uint8_t twi328P::write(uint8_t data)
{
	return 1;
}
// END twi328P::write()



//
//	Function:
//	Description:
//	Arguments:
//	Return value:
//
void twi328P::twiBusError(void){

}
//	END twi328P::twiBusError()



//
//	Function:
//	Description:
//	Arguments:
//	Return value:
//
void twi328P::twiPassedStatusCheck(void){

}
//	END twi328P::twiPassedStatusCheck()



//
//	Function:
//	Description:
//	Arguments:
//	Return value:
//
void twi328P::blinkLED(uint8_t _PORT, uint8_t count, uint8_t blinkNormal)
{

	if(!blinkNormal){
		for(uint8_t i = 0; i < 2*count; i++){
			//PORTB |= _PORT;
			PORTB ^= _PORT;
			_delay_us(25000);
			//PORTB &= ~_PORT;
			//_delay_us(32000);
		}
	}
	else{
		for(uint8_t i = 0; i < 2*count; i++){
			PORTB ^= _PORT;
			_delay_us(250000);
			//PORTB &= ~_PORT;
			//_delay_us(255000);
		}
	}
	_delay_us(210000);
}
//	END twi328P::blinkLED()



//
//	Function:		twi328P::checkStatusRegister()
//	Description:	checks the status register, TWSR, against the value passed by argument
//					it blinks the LED as a response, visualising an ok or error
//
//	Arguments:		it takes one argument, the status to check against
//
//	Return value:	_status
//
uint8_t twi328P::checkStatusRegister(uint8_t _status)
{
	return 1;
}
// END twi328P::checkStatusRegister()






/*
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
