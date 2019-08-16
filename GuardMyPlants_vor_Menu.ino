/*
#include <Arduino.h>
#include <LiquidCrystal.h>
//TODO aufraeumen, nachdem wir uns fuer Keypad entschieden haben
#include <Keypad.h>
#include <AnalogMatrixKeypad.h>
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"
#include "config.h"

int onOffSwitch = 0;
boolean run = false;
int minimalWaterLevel = 0;
double minimalMoistureLevelPlant1 = 0;
double minimalMoistureLevelPlant2 = 0;

//AnalogMatrixKeypad keypad(ANALOG_KEYPAD_PIN);

//TODO aufraeumen, nachdem wir uns fuer Keypad entschieden haben
// Kopie von custom_keypad.ino www.elegoo.com
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, {
		'7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

//TODO Print-Fehler (siehe Kommentar)
LiquidCrystal lcd(LCD_PINS[0], LCD_PINS[1], LCD_PINS[2], LCD_PINS[3], LCD_PINS[4], LCD_PINS[5]); // @suppress("Abstract class cannot be instantiated")
Keypad keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Entgegen der Spezifikation ist der max. Wert nicht 1024, sondern 320
WaterLevelSensor waterLevelSensor1(0, 320, WATER_LEVEL_SENSOR_PIN,
		minimalWaterLevel);
SoilMoistureSensor soilMoistureSensor1(0, 320, SOIL_MOISTURE_SENSOR_1_PIN,
		minimalMoistureLevelPlant1);
SoilMoistureSensor soilMoistureSensor2(0, 320, SOIL_MOISTURE_SENSOR_2_PIN,
		minimalMoistureLevelPlant2);
Waterpump waterpump1(WATERPUMP_1_PIN);
Waterpump waterpump2(WATERPUMP_2_PIN);

double waterLevelPercent;
int thresholdValuePercent;

void setup() {
	Serial.begin(9600);
	pinMode(RUN_SWITCH_PIN, INPUT);
	pinMode(ERROR_LED_PIN, OUTPUT);
	pinMode(RUN_LED_PIN, INPUT);
}

void loop() {
	lcd.print(DEBUG_GET_SERIAL_INPUT());
	onOffSwitch = digitalRead(RUN_SWITCH_PIN);
	if (onOffSwitch == HIGH)
	// Umschalten
	{
		delay(50); // Entprellen
		if (!run) {
			run = true;
			digitalWrite(RUN_LED_PIN, HIGH);

		} else {
			run = false;
			digitalWrite(RUN_LED_PIN, LOW);
		}
	}
	if (run) {
		//	waterLevelPercent = waterLevelSensor1.getPercentValue();
		//	delay(5000);
		char Key = keypad.getKey();
		if (Key != KEY_NOT_PRESSED) {
			Serial.println(Key);
		}
	}
}

String getSerialInput() {
	Serial.flush(); // Leert den seriellen Puffer
	return Serial.readString();
}

*/
