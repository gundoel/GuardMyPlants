#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(int minValue, int maxValue, int analogPin, double thresholdValuePercent): Sensor(minValue, maxValue, analogPin, thresholdValuePercent)
{
	pinMode(analogPin, INPUT);
}
