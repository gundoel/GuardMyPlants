#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(int minValue, int maxValue,
		int toleratedDeviation, int pin, double thresholdValuePercent) :
		Sensor(minValue, maxValue, toleratedDeviation, pin,
				thresholdValuePercent) {
	pinMode(pin, INPUT);
}
