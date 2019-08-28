/*
 * SoilMoistureSensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"
#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(int minValue, int maxValue, int analogPin, double neededMoisture1Percent) : Sensor(minValue, maxValue, analogPin, neededMoisture1Percent)
{
	pinMode(analogPin, INPUT);
}
