//
//	Function:		twi328P::write()
//
//	Description:
//
//
//
//
//	Arguments:		uint8_t data,	command or data to be written
//
//	return value:	returns status register with prescaler bits masked out: TWSR & 0xf8
//
uint8_t twi328P::write(uint8_t mode, uint8_t data)
{
	//
	//	if mode == 0, this means we're sending a command first, followed by some data
	//	we need to first write the command byte, which is 0, then the data byte
	//
	if(mode == 0){
		// TODO:	it is possible to do some error checking here, since twi328P::write() returns the status register
		write(0);
		write(data);
	}
	else{
		write(data);
	}

	return((TWSR & 0xf8));
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
//	Arguments:		uint8_t mode,	set to 0 for command or 1 for data
//					uint8_t len,	length of command/data buffer
//					uint8_t data,	command or data to be written
//
//	return value:	returns status register with prescaler bits masked out: TWSR & 0xf8
//
uint8_t twi328P::write(uint8_t data)
{
	//
	//	load the data into the TWDR
	//	and clear the TWINT bit to instruct hardware to start transmission of data
	//
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);


	waitForTWINTFlag();
	//checkStatusRegister(TWI_DATA_TRANSMITTED_ACK_RECEIVED);

	return(checkStatusRegister(TWI_DATA_TRANSMITTED_ACK_RECEIVED));
}
// END twi328P::write()
