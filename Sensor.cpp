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

Sensor::Sensor(int minValue, int maxValue, int analogPin, double thresholdValuePercent) :
minValue(minValue), maxValue(maxValue), analogPin(analogPin), thresholdValuePercent(thresholdValuePercent)  {
	pinMode(analogPin, INPUT);
}

double Sensor::getPercentValue() {
	DEBUG_PRINTLN(analogRead(analogPin));
	DEBUG_PRINTLN(calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
	return (calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
}

int Sensor::getAnalogPin() const {
	return analogPin;
}

int Sensor::getCurrentValue() const {
	return currentValue;
}

void Sensor::setCurrentValue(int currentValue) {
	this->currentValue = currentValue;
}

double Sensor::getThresholdValuePercent() const {
	return thresholdValuePercent;
}

void Sensor::setThresholdValuePercent(double thresholdValuePercent) {
	this->thresholdValuePercent = thresholdValuePercent;
}
