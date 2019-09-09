/*
 * Waterpump.cpp
 *
 * Created on: 07.09.2019
 *
 */

#include "Waterpump.h"
#include "debug.h"

Waterpump::Waterpump(byte pin) {
	pinMode(pin, OUTPUT);
}

Waterpump::~Waterpump() {}

// delivers the required quantity of water
void Waterpump::watering(int waterQuantity) {
	// depending on the requested quantity, the duty cycle is adapted in order to avoid overflow
	float wateringTime = 0;
	if (waterQuantity <= 20) {
		dutyCycle = dutyCycleSmall;
		wateringTime = waterQuantity/PumpCapacitySmall;
	}
	if (waterQuantity > 20 && waterQuantity <= 60) {
			dutyCycle = dutyCycleMedium;
			wateringTime = waterQuantity/PumpCapacityMedium;
		}
	if (waterQuantity > 60) {
			dutyCycle = dutyCycleLarge;
			wateringTime = waterQuantity/PumpCapacityLarge;
		}
	analogWrite(pin, dutyCycle);
	//DEBUG_PRINTLN(dutyCycle);
	delay(wateringTime * 1000);
	//DEBUG_PRINTLN(wateringTime);
	digitalWrite(pin, LOW);
}
