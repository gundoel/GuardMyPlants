/*
 * WaterLevelSensor.h
 *
 *  Created on: 19.07.2019
 *  Author: GMP Team
 *  Version: 1.0
 */

#ifndef WATERLEVELSENSOR_H_
#define WATERLEVELSENSOR_H_
#include "Sensor.h"

class WaterLevelSensor: public Sensor {
public:
	WaterLevelSensor(int lowValue, int highValue, int toleratedDeviation,
			byte pin);
};

#endif /* WATERLEVELSENSOR_H_ */
