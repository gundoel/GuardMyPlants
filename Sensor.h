/*
 * Sensor.h
 *
 *  Created on: 18.07.2019
 *  Author: GMP Team
 *  Version: 1.0
 */

#ifndef SENSOR_H_
#define SENSOR_H_

class Sensor {
protected:
	byte pin; // pin where sensor is connected
	int previousValue = 0;
	int lowValue; /* low value of sensor (can be a higher absolute value than highValue,
	e. g. moisture sensor returns high value for low moisture and vice versa */
	int highValue; // highest value of sensor
	int toleratedDeviation = 0;


public:
	Sensor(int lowValue, int highValue, int toleratedDeviation, byte pin);
	double getPercentValue();
	double getStableValue();
	int getId();
};

#endif /* SENSOR_H_ */
