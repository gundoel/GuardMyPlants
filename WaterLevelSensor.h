/*
 * WaterLevelSensor.h
 *
 *  Created on: 19.07.2019
 *      Author: TAARISI3
 */

#ifndef WATERLEVELSENSOR_H_
#define WATERLEVELSENSOR_H_
#include "Sensor.h"

class WaterLevelSensor : public Sensor {
	public:
		WaterLevelSensor(int minValue, int maxValue, int thresholdValuePercent, char analogPin);
		void printValue(int value);
		double getWaterLevel();
		boolean isWaterLevelOK();

};

#endif /* WATERLEVELSENSOR_H_ */
