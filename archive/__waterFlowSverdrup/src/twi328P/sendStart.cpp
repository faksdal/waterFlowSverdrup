

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
	//uint8_t	retFlag = 0b00000000;


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

	waitForTWINTFlag();

	if(checkStatusRegister(TWI_START_TRANSMITTED) == TWI_START_TRANSMITTED){
		blinkLED(0b00100000, 20, 1);
		_delay_us(500000);
		return(TWI_START_TRANSMITTED);
	}
	else if(checkStatusRegister(TWI_START_TRANSMITTED) == TWI_START_TRANSMITTED-1){
		blinkLED(0b00100000, 15, 1);
		_delay_us(500000);
		return(TWI_START_TRANSMITTED - 1);
	}

	return(TWI_START_TRANSMITTED);

}
