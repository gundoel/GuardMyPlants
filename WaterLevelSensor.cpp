#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(int lowValue, int highValue,
		int toleratedDeviation, byte pin) :
		Sensor(lowValue, highValue, toleratedDeviation, pin) {
	pinMode(pin, INPUT);
}
