/*
 * SoilMoistureSensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"
#include "SoilMoistureSensor.h"
#include "gmputil.h"

SoilMoistureSensor::SoilMoistureSensor(int minValue, int maxValue, int analogPin) : Sensor(minValue, maxValue, analogPin)
{
	pinMode(analogPin, INPUT);
}

double SoilMoistureSensor::getPercentValue() {
	return gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin));
}
