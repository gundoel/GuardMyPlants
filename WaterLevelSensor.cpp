#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"
#include "gmputil.h"

WaterLevelSensor::WaterLevelSensor(int minValue, int maxValue, int analogPin): Sensor(minValue, maxValue, analogPin)
{
	pinMode(analogPin, INPUT);
}

double WaterLevelSensor::getPercentValue() {
	return gmputil::calculatePercentValue(minValue, maxValue, analogRead(analogPin));
}
