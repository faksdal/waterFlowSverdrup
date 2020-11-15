//
//	Function:
//	Description:
//	Arguments:
//	Return value:
//
void twi328P::twiBusError(void){
	//blinkLED(0b00100000, 5, 0);
	//_delay_us(250000);
	//blinkLED(0b00100000, 5, 0);
	//_delay_us(500000);
}
//	END twi328P::twiBusError()



//
//	Function:
//	Description:
//	Arguments:
//	Return value:
//
void twi328P::twiPassedStatusCheck(void){
	//blinkLED(0b00100000, 1, 1);
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
	if ((TWSR & 0xf8) != _status){
		twiBusError();
		return(_status - 1);
	}
	else
		twiPassedStatusCheck();

	return(_status);
}
// END twi328P::checkStatusRegister()



