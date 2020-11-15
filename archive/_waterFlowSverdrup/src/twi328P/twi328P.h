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
#define	ERROR_RESTART_MAX	0b0000001;


void blinkLED(uint8_t _PORT, uint8_t count, uint8_t blinkNormal);


class twi328P{

	enum twiMode {none, masterTransmitter, masterReceiver, slaveTransmitter, slaveReceiver};



	twiMode		mode;

	uint8_t		reStarts;
	uint8_t		twsrStatus(void);


public:
	twi328P(void);

	uint8_t	init(uint32_t _busSpeed);
	void	disable(void);
	uint8_t	sendStart(void);
	uint8_t	mtMode(uint8_t slaveAddress);
	void	stop(void);
	uint8_t	write(uint8_t _slaveAddress, uint8_t address, uint8_t data);//, i2cMode _mode);
	//uint8_t	write(uint8_t _slaveAddress, uint8_t control, uint8_t data);


};


#endif /* ATMEGA328P_WATERFLOWSVERDRUP_SRC_TWI_TWI328P_H_ */
