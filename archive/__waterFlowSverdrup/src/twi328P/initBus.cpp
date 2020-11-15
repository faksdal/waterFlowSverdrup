//
//	Function:		twi328P::initBus()
//
//	Description:	initializes the i2c/twi bus, making it ready for transmission
//					sets TWBR according to the desired bus speed based on formula on p.222 in the data sheet
//					it also sets the prescaler bits in the TWSR, p.241 in the data sheet
//
//	arguments:		uint32_t, the desired bus speed 100 000 Hz, 400 000 Hz, 1 000 000 Hz or 3 200 000 Hz
//
//	return value:	ERROR_BUS_SPEED_OUT_OF_RANGE if the desired bus speed is not within i2c specs
//					returns status register with prescaler bits masked out: TWSR & 0xf8
//
/*
 *
void twi_init(void)
{
  // initialize state
  twi_state = TWI_READY;

  // activate internal pullups for twi.
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);

  // initialize twi prescaler and bit rate
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

  // twi bit rate formula from atmega128 manual pg 204
  //SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
  //note: TWBR should be 10 or higher for master mode
  //It is 72 for a 16mhz Wiring board with 100kHz TWI

  // enable twi module, acks, and twi interrupt
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}
*/

uint8_t	twi328P::initBus(uint32_t _busSpeed)	// pass the desired i2c bus speed as parameter
{
	//
	//	standard mode:	100	kbit/s
	//	full speed:		400	kbit/s
	//	fast mode:		1	Mbit/s
	//	high speed:		3,2	Mbit/s
	//
	// if the user don't pass one of the supported frequencies, return with an error
	//
	if(_busSpeed != 100000UL && _busSpeed != 400000UL && _busSpeed != 1000000UL && _busSpeed != 3200000UL)
		return(TWI_ERROR_BUS_SPEED_OUT_OF_RANGE);

	//
	//	making sure the power redution register (PRR), bit 7 is set to zero
	//	this is the power reduction for TWI
	PRR &= ~(1 << PRTWI);

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

	//
	//	enable twi module
	//	disable interrupts
	//	enable acknowledgement
	//
	TWCR = (1 << TWEN) | ~(1 << TWIE) | (1 << TWEA);

	twiBusMode = busInitialiseSent;

	return(TWI_BUS_INITIALISE_SENT);
}
//	END twi328P::initBus()
