#include <ThreadController.h>
#include "WaterLevelSensor.h"
#include <Arduino.h>
#include <Keypad.h> //TODO evtl. kriegt man es hin, abhaengig von config.h nur 1 der beiden Libraries einzubinden
#include <AnalogMatrixKeypad.h>
#include "SoilMoistureSensor.h"
#include "Waterpump.h"

#include "config.h"

const int WATER_LEVEL_SENSOR_PIN = A0;
const int SOIL_MOISTURE_SENSOR_1_PIN = A1;
const int SOIL_MOISTURE_SENSOR_2_PIN = A2;
const int WATERPUMP_1_PIN = A3;
const int WATERPUMP_2_PIN = A4;

//TODO aufraeumen, nachdem wir uns fuer Keypad entschieden haben (inkl. initKeypad entfernen)
//AnalogMatrixKeypad keypad(ANALOG_KEYPAD_PIN);
//const int P_ANALOG_KEYPAD_PIN = A3;

// Kopie von custom_keypad.ino www.elegoo.com
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] =
{
		{ '1', '2', '3', 'A' },
		{ '4', '5', '6', 'B' },
		{'7', '8', '9', 'C' },
		{ '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad keypad (makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Entgegen der Spezifikation ist der max. Wert nicht 1024, sondern 320
WaterLevelSensor waterLevelSensor1(0, 320, WATER_LEVEL_SENSOR_PIN);
SoilMoistureSensor soilMoistureSensor1(0, 320, SOIL_MOISTURE_SENSOR_1_PIN);
SoilMoistureSensor soilMoistureSensor2(0, 320, SOIL_MOISTURE_SENSOR_1_PIN);
Waterpump waterpump1(WATERPUMP_1_PIN);
Waterpump waterpump2(WATERPUMP_2_PIN);

double waterLevelPercent;
int thresholdValuePercent;

void setup() {
	Serial.begin(9600);
	pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);
	pinMode(SOIL_MOISTURE_SENSOR_1_PIN, INPUT);
	pinMode(SOIL_MOISTURE_SENSOR_1_PIN, INPUT);
}

void loop() {
//	waterLevelPercent = waterLevelSensor1.getPercentValue();
//	delay(5000);
	char Key = keypad.getKey();
	if (Key != KEY_NOT_PRESSED) {
		Serial.println(Key);
	}
}

