/*
 * SoilMoistureSensor.cpp
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Sensor.h"
#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(int minValue, int highValue,
		int toleratedDeviation, byte pin) :
		Sensor(minValue, highValue, toleratedDeviation, pin) {
	pinMode(pin, INPUT);
}
