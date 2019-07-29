#include <ThreadController.h>
#include "WaterLevelSensor.h"
#include "SoilMoistureSensor.h"
#include <Arduino.h>

int startButton = 13;
// Entgegen der Spezifikation ist der max. Wert nicht 1024, sondern 320
WaterLevelSensor waterLevelSensor1 (0, 320, A0);
SoilMoistureSensor soilMoistureSensor1 (0, 320, A1);
SoilMoistureSensor soilMoistureSensor2 (0, 320, A2);

double waterLevelPercent;
int thresholdValuePercent;

void setup() {
	pinMode(A0, INPUT);
	pinMode(startButton, INPUT);

	Serial.begin(9600);
//	ThreadController controller = ThreadController();
//	controller.add(waterLevelSensor1.getThread());
//	controller.run();
}

void loop() {
//	if(digitalRead(startButton) == HIGH) {
//		waterLevelSensor1.activate();
//	}
	waterLevelPercent = waterLevelSensor1.getPercentValue();
}
