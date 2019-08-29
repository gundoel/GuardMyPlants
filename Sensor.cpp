/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"
#include "config.h"
#include "gmputil.hpp"

using namespace gmp_math_utils;

Sensor::Sensor(int minValue, int maxValue, int toleratedDeviation,
		int pin, double thresholdValuePercent) :
		minValue(minValue), maxValue(maxValue), toleratedDeviation(
				toleratedDeviation), pin(pin), thresholdValuePercent(
				thresholdValuePercent) {
	pinMode(pin, INPUT);
}

double Sensor::getPercentValue() {
	DEBUG_PRINTLN("Sensor value " + String(getStableValue()));
	DEBUG_PRINTLN("Sensor % " + String(
			map(getStableValue(), minValue, maxValue, 0, 100)));
	return map(getStableValue(), minValue, maxValue, 0, 100);
}

/* measured value needs to be stabilized using previous value and tolerated deviation
 * values below 0 are ignored
 */
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

double Sensor::getThresholdValuePercent() {
	return thresholdValuePercent;
}

void Sensor::setThresholdValuePercent(double thresholdValuePercent) {
	this->thresholdValuePercent = thresholdValuePercent;
}
