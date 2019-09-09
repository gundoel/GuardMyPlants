/*
 * Sensor.h
 *
 *  Created on: 18.07.2019
 *      Author: TAARISI3
 */

#ifndef SENSOR_H_
#define SENSOR_H_

class Sensor {
protected:
	int previousValue = 0;
	int lowValue; /* low value of sensor (can be a higher absolute value than highValue,
	e. g. moisture sensor returns high value for low moisture and vice versa */
	int highValue; // highest value of sensor
	int toleratedDeviation = 0;
	byte pin; // pin where sensor is connected

public:
	Sensor(int lowValue, int highValue, int toleratedDeviation, byte pin);
	double getPercentValue();
	double getStableValue();
};

#endif /* SENSOR_H_ */
