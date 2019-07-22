#include "WaterLevelSensor.h"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <stdio.h>

WaterLevelSensor::WaterLevelSensor(int minValue, int maxValue, int criticalLevelPercent, char analogPin) {
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->analogPin = analogPin;
	this->criticalLevelPercent = criticalLevelPercent;
}

void boringCallback() {
	Serial.println("BORING...");
}

void WaterLevelSensor::activate() {
	thread = Thread();
	thread.setInterval(10000);
	thread.onRun(boringCallback);
}

void WaterLevelSensor::checkWaterLevel() {
	int value = analogRead(analogPin);
	if (((historyValue >= value) && ((historyValue - value) > 10))
			|| ((historyValue < value) && ((value - historyValue) > 10))) {
		historyValue = value;
		printValue(value);
	}
}

Thread * WaterLevelSensor::getThread() {
	return &thread;
}

void WaterLevelSensor::printValue(int value) {
	sprintf(printBuffer, "Wasserstand: %d", value);
}
