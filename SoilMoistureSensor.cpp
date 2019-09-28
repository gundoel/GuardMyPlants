/*
 * SoilMoistureSensor.cpp
 *
 *  Created on: 29.07.2019
 *  Author: GMP Team
 *  Version: 1.0
 */

#include <Arduino.h>
#include "Sensor.h"
#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(int minValue, int highValue,
		int toleratedDeviation, byte pin) :
		Sensor(minValue, highValue, toleratedDeviation, pin) {
	pinMode(pin, INPUT);
}
