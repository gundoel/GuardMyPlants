#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(int lowValue, int highValue,
		int toleratedDeviation, byte pin, int id) :
		Sensor(lowValue, highValue, toleratedDeviation, pin, id) {
	pinMode(pin, INPUT);
}
