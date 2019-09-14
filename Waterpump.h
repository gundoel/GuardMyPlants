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
	int dutyCycle = 0; // pump duty cycle. Low duty cycle for small amounts of water, higher value for bigger amounts.
	int id;
	byte pin; // pin where pump is connected
	boolean isPumpRunning = false;


public:
	Waterpump(byte pin, int id);
	virtual ~Waterpump();
	int Waterpump::getId();
	boolean getIsPumpRunning();
	void startWatering(int dutyCycle);
	void stopWatering();
	void setIsPumpRunning(boolean isPumpRunning);
};

#endif /* WATERPUMP_H_ */
