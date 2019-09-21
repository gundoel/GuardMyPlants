/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"

#include "debug.h"
#include "gmputil.hpp"

using namespace gmp_math_utils;

Sensor::Sensor(int lowValue, int highValue, int toleratedDeviation, byte pin, int id) :
		lowValue(lowValue), highValue(highValue), toleratedDeviation(
				toleratedDeviation), pin(pin), id(id) {
	pinMode(pin, INPUT);
}

double Sensor::getPercentValue() {
	return map(getStableValue(), lowValue, highValue, 0, 100);
}

int Sensor::getId() {
	return id;
}

// measured value needs to be stabilized using previous value and tolerated deviation
double Sensor::getStableValue() {
	int currentValue = analogRead(pin);
	if (((previousValue >= currentValue)
			&& ((previousValue - currentValue) > toleratedDeviation))
			|| ((previousValue < currentValue)
					&& ((currentValue - previousValue) > toleratedDeviation))) {
		previousValue = currentValue;
	}
	return previousValue;
}
