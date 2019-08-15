/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"
#include "gmputil.h"
#include "config.h"

Sensor::Sensor(int minValue, int maxValue, int analogPin, double thresholdValuePercent) :
minValue(minValue), maxValue(maxValue), analogPin(analogPin), thresholdValuePercent(thresholdValuePercent)  {
	pinMode(analogPin, INPUT);
}

double Sensor::getPercentValue() {
	DEBUG_PRINTLN(analogRead(analogPin));
	DEBUG_PRINTLN(gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
	return (gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
}

int Sensor::getAnalogPin() const {
	return analogPin;
}

int Sensor::getCurrentValue() const {
	return currentValue;
}

void Sensor::setCurrentValue(int currentValue = 0) {
	this->currentValue = currentValue;
}

double Sensor::getThresholdValuePercent() const {
	return thresholdValuePercent;
}

void Sensor::setThresholdValuePercent(double thresholdValuePercent) {
	this->thresholdValuePercent = thresholdValuePercent;
}
