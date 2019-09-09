/*
 * Waterpump.h
 *
 * Created on: 07.09.2019
 *
 */

#ifndef WATERPUMP_H_
#define WATERPUMP_H_
#include "Arduino.h"

class Waterpump {

private:
	int dutyCycle = 0; // pump duty cycle. Low duty cycle for small amounts of water, higher value for bigger amounts.
	byte pin; // pin where pump is connected
	int dutyCycleSmall = 50; // Duty Cycle for small amounts
	int dutyCycleMedium = 100; // Duty Cycle for medium amounts
	int dutyCycleLarge = 150; // Duty Cycle for large amounts
	float PumpCapacitySmall = 6; // 6 ml/s
	float PumpCapacityMedium = 19; // 19 ml/s
	float PumpCapacityLarge = 23; // 23 ml/s

public:
	Waterpump(byte pin);
	virtual ~Waterpump();
	void watering(int waterQuantity);
};

#endif /* WATERPUMP_H_ */
