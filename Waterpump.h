/*
 * Waterpump.h
 *
 * Created on: 07.09.2019
 *
 */

#ifndef WATERPUMP_H_
#define WATERPUMP_H_
#include <Timer.h>
#include "Arduino.h"

class Waterpump {

private:
	/* pump duty cycle. Low duty cycle for small amounts of water,
	higher value for bigger amounts.*/
	int dutyCycle = 0;
	// pin where pump is connected
	byte pin;
	// pumps are not ready if running or gmp is waiting for water to seep away
	boolean isPumpReady = true;


public:
	Waterpump(byte pin);
	virtual ~Waterpump();
	boolean getIsPumpReady();
	void startWatering(int dutyCycle);
	void stopWatering();
	void setIsPumpReady(boolean isPumpReady);
};

#endif /* WATERPUMP_H_ */
