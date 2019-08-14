/*
 * Sensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include "Sensor.h"
#include "gmputil.h"
#include <Arduino.h>

#define DEBUG //
#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

Sensor::Sensor(int minValue, int maxValue, int analogPin) {
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->analogPin = analogPin;
	this->thresholdValuePercent = thresholdValuePercent;
}

double Sensor::getPercentValue() {
	DEBUG_PRINTLN(analogRead(analogPin));
	DEBUG_PRINTLN(gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
	return (gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin)));
}
