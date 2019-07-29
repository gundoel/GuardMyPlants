#include <Arduino.h>
#include "Sensor.h"
#include "WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(int minValue, int maxValue, char analogPin) : Sensor(minValue, maxValue, analogPin) {
}
