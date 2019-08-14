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
		int analogPin; // Pin, an dem der Sensor angeschlossen ist
		int currentValue = 0; // Aktueller Messwert
		int historyValue = 0; // Vorheriger Messwert
		int minValue; // Hoechster Wert, den der Sensor liefert.
		int maxValue; // Tiefster Wert, den der Sensor liefert.
		double thresholdValuePercent; //Schwellwert für Alarm/Aktion

	public:
		Sensor(int minValue, int maxValue, int analogPin);
		double getPercentValue();
};

#endif /* SENSOR_H_ */
