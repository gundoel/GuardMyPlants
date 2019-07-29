/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include "Sensor.h"

Sensor::Sensor(int minValue, int maxValue, int thresholdValuePercent, char analogPin) {
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->analogPin = analogPin;
	this->thresholdValuePercent = thresholdValuePercent;
}
