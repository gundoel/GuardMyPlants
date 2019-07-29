#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"
#include "gmputil.h"

WaterLevelSensor::WaterLevelSensor(int minValue, int maxValue, int thresholdValuePercent, char analogPin) : Sensor(minValue, maxValue, thresholdValuePercent, analogPin) {
}

double WaterLevelSensor::getWaterLevel() {
	return (gmputil::calculatePercentValue(10, 10, analogRead(analogPin)));
}
