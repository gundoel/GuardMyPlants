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
	int currentValue = 0; // Aktueller Messwert
	int minValue; // Hoechster Wert, den der Sensor liefert.
	int maxValue; // Tiefster Wert, den der Sensor liefert.
	int analogPin; // Pin, an dem der Sensor angeschlossen ist
	double thresholdValuePercent; //Schwellwert für Alarm/Aktion

public:
	Sensor(int minValue, int maxValue, int analogPin,
			double thresholdValuePercent);
	double getPercentValue();
	int getAnalogPin() const;
	int getCurrentValue() const;
	void setCurrentValue(int currentValue = 0);
	double getThresholdValuePercent() const;
	void setThresholdValuePercent(double thresholdValuePercent);
};

#endif /* SENSOR_H_ */
