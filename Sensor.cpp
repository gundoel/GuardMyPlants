/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include "Sensor.h"
#include "gmputil.h"
#include <Arduino.h>

Sensor::Sensor(int minValue, int maxValue, char analogPin) {
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->analogPin = analogPin;
	this->thresholdValuePercent = thresholdValuePercent;
}

double Sensor::getPercentValue() {
	return (gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
}
