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
	int toleratedDeviation = 0;
	int minValue; // Hoechster Wert, den der Sensor liefert.
	int maxValue; // Tiefster Wert, den der Sensor liefert.
	int pin; // Pin, an dem der Sensor angeschlossen ist
	double thresholdValuePercent; //Schwellwert für Alarm/Aktion

public:
	Sensor(int minValue, int maxValue, int toleratedDeviation, int pin,
			double thresholdValuePercent);
	double getPercentValue();
	double getStableValue();
	double getThresholdValuePercent();
	void setThresholdValuePercent(double thresholdValuePercent);
};

#endif /* SENSOR_H_ */
