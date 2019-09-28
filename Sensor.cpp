/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *  Author: GMP Team
 *  Version: 1.0
 */

#include <Arduino.h>
#include "Sensor.h"

#include "debug.h"
#include "gmputils.hpp"

using namespace gmp_math_utils;

Sensor::Sensor(int lowValue, int highValue, int toleratedDeviation, byte pin) :
		lowValue(lowValue), highValue(highValue), toleratedDeviation(
				toleratedDeviation), pin(pin) {
	pinMode(pin, INPUT);
}

double Sensor::getPercentValue() {
	/* due to variable sensor ranges values could turn out negative,
	/ therefore minimum 0 */
	return max(map(getStableValue(), lowValue, highValue, 0, 100), 0);
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
