#include <Arduino.h>
#include <LiquidCrystal.h>
//TODO aufraeumen, nachdem wir uns fuer Keypad entschieden haben
#include <Keypad.h>
#include <AnalogMatrixKeypad.h>
#include <map>
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"
#include "config.h"
#include "gmputil.hpp"

using namespace gmp_string_utils;
using namespace gmp_keypad_utils;

int onOffSwitch = 0;
boolean run = false;
int minimalWaterLevel = 0;
double minimalMoistureLevelPlant1 = 0;
double minimalMoistureLevelPlant2 = 0;

std::map<byte,byte> ASCIIMap = buildASCIIMap();
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
//Waterpump waterpump1(WATERPUMP_1_PIN);
//Waterpump waterpump2(WATERPUMP_2_PIN);

double waterLevelPercent;
int thresholdValuePercent;

// Menu begin
byte buttons[] = {6,7,8}; //10 for additional pin
const byte nrButtons = 3; // change to 4 if additional button added
int menusize = 10;
String menu[] = {
  "Menu",                   //0
  "Menu>LED",               //1
  "Menu>LED>Off",           //2
  "Menu>LED>On",            //3
  "Menu>LED>Fade",          //4
  "Menu>LED>Blink",         //5
  "Menu>LCDlight",          //6
  "Menu>Nothing1",          //7
  "Menu>Nothing2",          //8
  "Menu>Nothing3"           //9
};

int t;
byte pressedButton, currentPos,currentPosParent, possiblePos[20], possiblePosCount, possiblePosScroll = 0;
String parent = "";

int brightness;
int LCDtoggle = 0;
int fadeAmount = 5;
unsigned long timer = (millis() / 10);
int ledMode = 0;

void updateMenu () {
  possiblePosCount = 0;
  while (possiblePosCount == 0) {

    for (t = 1; t < menusize;t++) {
      if (mid(menu[t],1,inStrRev(menu[t],">")-1).equals(menu[currentPos])) {
        possiblePos[possiblePosCount]  =  t;
        possiblePosCount = possiblePosCount + 1;
      }
    }

    //find the current parent for the current menu
    parent = mid(menu[currentPos],1,inStrRev(menu[currentPos],">")-1);
    currentPosParent = 0;
    for (t = 0; t < menusize; t++) {
       if (parent == menu[t]) {currentPosParent = t;}
    }

    // reached the end of the Menu line
    if (possiblePosCount == 0) {
      //Menu Option Items
      switch (currentPos) {
        case 2:
        case 3:
        case 4:
        case 5://Choose between 2,3,4,5
          for (t = 2; t<6; t++) {
            if (mid(menu[t],len(menu[t]),1) == "*") {
              menu[t] = mid(menu[t],1,len(menu[t])-1);
            }
          }
          menu[currentPos] = menu[currentPos] + "*";
        break;
        case 6: //Toggle
          if (mid(menu[currentPos],len(menu[currentPos]),1) == "*") {
            menu[currentPos] = mid(menu[currentPos],1,len(menu[currentPos])-1);
          } else {
            menu[currentPos] = menu[currentPos] + "*";
          }

        break;
      }

      //Set Variables
      switch (currentPos) {
        case 2:
          ledMode = 0;
        break;
        case 3:
          ledMode = 1;
        break;
        case 4:
          brightness = 128;
          ledMode = 2;
        break;
        case 5:
          brightness = 0;
          ledMode = 3;
        break;
        case 6:
          LCDtoggle = abs(LCDtoggle - 1);
          digitalWrite(13,LCDtoggle);
        break;
      }


      // go to the parent
      currentPos = currentPosParent;
    }
  }
    lcd.clear();
    lcd.setCursor(0,0); lcd.print(mid(menu[currentPos],inStrRev(menu[currentPos],">")+1,len(menu[currentPos])-inStrRev(menu[currentPos],">")));
    lcd.setCursor(0,1); lcd.print(mid(menu[possiblePos[possiblePosScroll]],inStrRev(menu[possiblePos[possiblePosScroll]],">")+1,len(menu[possiblePos[possiblePosScroll]])-inStrRev(menu[possiblePos[possiblePosScroll]],">")));

}

// Look for a button press
byte checkButtonPress() {
  byte bP = 0;
  byte rBp = 0;
  // Necessary. Using read data directly in function will not work
  String input;
  // rizzisim: for development, when no keypad is connected
  if(Serial.available() > 0) {
	  input = Serial.readString();
	  DEBUG_PRINTLN(input);
	  bP = ASCIIMap.find(input[0]);
  }
  else {
	  for (t = 0; t<nrButtons;t++) {
		// rizzisim: buttons defined as pullup, therefore 0 when pressed
		if (digitalRead(buttons[t]) == 0) {bP = (t + 1);}
	  }
  }
  rBp = bP;
  while (bP != 0) { // wait while the button is still down
    bP = 0;
    for (t = 0; t<nrButtons;t++) {
      if (digitalRead(buttons[t]) == 0) {bP = (t + 1);}
    }
  }
  return rBp;
}
// Menu end

void setup() {
	Serial.begin(9600);
	Serial.flush(); // Leert den seriellen Puffer
	pinMode(RUN_SWITCH_PIN, INPUT);
	pinMode(ERROR_LED_PIN, OUTPUT);
	pinMode(RUN_LED_PIN, INPUT);

	// Menu begin
	  lcd.begin(16,2);
	  lcd.clear();

	  for (t=0;t<nrButtons;t++) {
	    pinMode(buttons[t],INPUT_PULLUP);
	  }

	  pinMode(13,OUTPUT);
	  pinMode(9,OUTPUT);

	  lcd.setCursor(0,0); lcd.print("HELLO");
	  delay(1000);
	  updateMenu();
	// Menu end
}

void loop() {
// Menu begin
	  switch (ledMode) {
	    case 0:
	      analogWrite(3, 0);
	    break;
	    case 1:
	      analogWrite(3, 255);
	    break;
	    case 3:
	      if ((millis() / 10) - timer > 30) {
	        timer = (millis() / 10);
	        brightness = abs(brightness - 255);
	        timer = (millis() / 10);
	        analogWrite(3, brightness);
	      }
	    break;
	    case 2:
	      if ((millis() / 10) - timer > 3) {
	        timer = (millis() / 10);
	        analogWrite(3, brightness);
	        brightness = brightness + fadeAmount;
	        if (brightness <= 0 || brightness >= 255) {
	          fadeAmount = -fadeAmount;
	        }
	      }
	    break;
	  }

	  pressedButton = checkButtonPress();
	  if (pressedButton !=0) {
		DEBUG_PRINTLN(pressedButton);
	    switch (pressedButton) {
	      case 1:
	          possiblePosScroll = (possiblePosScroll + 1) % possiblePosCount; // Scroll
	      break;
	      // If I wanted a 4 button controll of the menu
	      //case 4:
	        //  possiblePosScroll = (possiblePosScroll + possiblePosCount - 1) % possiblePosCount; // Scroll
	      //break;
	      case 2:
	        currentPos = possiblePos[possiblePosScroll]; //Okay
	      break;
	      case 3:
	        currentPos = currentPosParent; //Back
	        possiblePosScroll = 0;
	      break;
	    }
	    updateMenu();
	  }
// Menu end

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
	return Serial.readString();
}

