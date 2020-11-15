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
