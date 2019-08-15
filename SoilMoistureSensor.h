/*
 * SoilMoistureSensor.h
 *
 *  Created on: 29.07.2019
 *      Author: TAARISI3
 */

#ifndef SOILMOISTURESENSOR_H_
#define SOILMOISTURESENSOR_H_

#include "Sensor.h"

class SoilMoistureSensor: public Sensor {
public:
	SoilMoistureSensor(int minValue, int maxValue, int analogPin, double thresholdValuePercent);
	double getPercentValue();
};

#endif /* SOILMOISTURESENSOR_H_ */
