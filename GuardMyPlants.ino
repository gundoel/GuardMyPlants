#include <ThreadController.h>
#include "WaterLevelSensor.h"
#include <Arduino.h>

int startButton = 13;
WaterLevelSensor waterLevelSensor1 (0, 1024, 10, A0);

void setup() {
	pinMode(A0, INPUT);
	pinMode(startButton, INPUT);

	Serial.begin(9600);
	ThreadController controller = ThreadController();
	controller.add(waterLevelSensor1.getThread());
	controller.run();
}

void loop() {
	if(digitalRead(startButton) == HIGH) {
		waterLevelSensor1.activate();
	}
}
