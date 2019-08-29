/*
 * SoilMoistureSensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"
#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(int minValue, int maxValue,
		int toleratedDeviation, int pin, double neededMoisture1Percent) :
		Sensor(minValue, maxValue, toleratedDeviation, pin,
				neededMoisture1Percent) {
	pinMode(pin, INPUT);
}
