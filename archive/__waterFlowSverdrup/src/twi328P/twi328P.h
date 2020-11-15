/*
 * twi328P.h
 *
 *  Created on: 31 Oct 2020
 *      Author: jole
 */

#ifndef ATMEGA328P_WATERFLOWSVERDRUP_SRC_TWI_TWI328P_H_
#define ATMEGA328P_WATERFLOWSVERDRUP_SRC_TWI_TWI328P_H_

#ifndef F_CPU
	#define F_CPU			16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdint.h>

#define MAX_RESTARTS		30

#define	TWI_OK								0b11000000
#define TWI_BUS_INITIALISE_SENT				0b11100000
#define	TWI_ERROR_RESTART_MAX				0b00000001
#define	TWI_ERROR_BUS_SPEED_OUT_OF_RANGE	0b10000000


//
//	twi status codes, to be read from the TWSR
//
#define TWI_START_TRANSMITTED				0b00001000	//	0x08
#define TWI_REPEATED_START_TRANSMITTED		0b00010000	//	0x10
#define TWI_SLA_W_TRANSMITTED_ACK_RECEIVED	0b00011000	//	0x18
#define TWI_DATA_TRANSMITTED_ACK_RECEIVED	0b00101000	//	0x28


class twi328P{

	//
	//	used to keep track of which mode we're in
	//
	enum twiMode {none, busInitialiseSent, masterTransmitter, masterReceiver, slaveTransmitter, slaveReceiver};



	twiMode		twiBusMode;
	uint8_t		reStarts;			// keep tracks of number of bus restarts. We don't want the chip to hang in an infinite loop...

	//uint8_t		twsrStatus(void);


	//void		blinkLED(uint8_t _PORT, uint8_t count, uint8_t blinkNormal);
	void		twiBusError(void);
	void		twiPassedStatusCheck(void);
	void		waitForTWINTFlag(void);

public:
	twi328P(void);

	void		blinkLED(uint8_t _PORT, uint8_t count, uint8_t blinkNormal);

	uint8_t	checkStatusRegister(uint8_t _status);
	void	disableBus(void);


	uint8_t	initBus(uint32_t _busSpeed);
	uint8_t	sendStart(void);
	uint8_t	sendSlaW(uint8_t _slaveAddress);
	uint8_t	write(uint8_t mode, uint8_t data);
	uint8_t	write(uint8_t data);
	void	sendStop(void);






	//
	//uint8_t	mtMode(uint8_t slaveAddress);
	//void	stop(void);
	//uint8_t	write(uint8_t _slaveAddress, uint8_t address, uint8_t data);//, i2cMode _mode);
	//uint8_t	write(uint8_t _slaveAddress, uint8_t control, uint8_t data);


};



#endif /* ATMEGA328P_WATERFLOWSVERDRUP_SRC_TWI_TWI328P_H_ */
