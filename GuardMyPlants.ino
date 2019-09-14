#include "debug.h"
#include <Arduino.h>
#include <AnalogMatrixKeypad.h>
#include <LiquidCrystal.h>
#include <Timer.h>
#include "config.h"
#include "MenuData.h"
#include "Sensor.h"
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"
#include "Log.h"
#include "gmputil.hpp"

using namespace gmp_keypad_utils;
using namespace gmp_string_utils;
using namespace gmp_math_utils;

// Initialize with default values defined in config.h
Potsize potSize1 = DEFAULT_POT_SIZE;
Potsize potSize2 = DEFAULT_POT_SIZE;
Moisture neededMoisture1Percent = DEFAULT_MOISTURE;
Moisture neededMoisture2Percent = DEFAULT_MOISTURE;

Timer timerPump1;
Timer timerPump2;

int onOffSwitch = 0;
int startRunTime = 0;
boolean run = false;
boolean error = false;
char strbuf[LCD_COLS + 1]; // one line of lcd display

// initialize lcd with interface pins defined in config.h
LiquidCrystal lcd(LCD_PIN1, LCD_PIN2, LCD_PIN3, LCD_PIN4, LCD_PIN5, LCD_PIN6);

// initialize keypad with interface pin defined in config.h
AnalogMatrixKeypad keypad(4);
// TODO use setter to set thresholdvalue
//keypad.setThresholdValue(10);

/* initialize sensors with interface pins defined in config.h
 * Tested max return value of water level sensor is 320, not 1024 as mentioned in specs.
 * Tested min value is 40.
 */
WaterLevelSensor waterLevelSensor(0, 336, 10, WATER_LEVEL_SENSOR_PIN);
// soil moisture sensor return value high when moisture low
SoilMoistureSensor soilMoistureSensor1(1015, 280, 10, SOIL_MOISTURE_SENSOR_1_PIN);
SoilMoistureSensor soilMoistureSensor2(1015, 280, 10, SOIL_MOISTURE_SENSOR_2_PIN);
Waterpump waterpump1(WATERPUMP_1_PIN);
Waterpump waterpump2(WATERPUMP_2_PIN);

/**************************************************************************************************
 * MENU SETUP AND FUNCTIONS
 **************************************************************************************************/
byte btn; // button

enum AppModeValues {
	APP_NORMAL_MODE, APP_MENU_MODE, APP_PROCESS_MENU_CMD
};

// Convert integer to string. Doesn't really belong here, should be somewhere else.
extern char *inttostr(char *dest, short integer);

// Apply left padding to string.
extern char *lpad(char *dest, const char *str, char chr = ' ',
		unsigned char width = LCD_COLS);
// Apply right padding to string.
extern char *rpad(char *dest, const char *str, char chr = ' ',
		unsigned char width = LCD_COLS);
// Apply string concatenation. argc = number of string arguments to follow.
extern char *fmt(char *dest, unsigned char argc, ...);

//TODO move functions to some header
void refreshMenuDisplay(byte refreshMode);
byte getNavAction();

byte appMode = APP_NORMAL_MODE;
MenuManager Menu1(GMPMenu_Root, menuCount(GMPMenu_Root));

//TODO move functions to some header file
void showDefaultScreen() {
	lcd.clear();
	lcd.print(DEFAULT_SCREEN);
}

void showStringMessage(String message, int line, int offset) {
	lcd.setCursor(offset, line);
	char buf[LCD_COLS];
	// TODO scrolling for longer messages
	message.toCharArray(buf, len(message) + 1);
	lcd.print(rpad(strbuf, buf));
	lcd.print(message);
}

// returns button either from Serial (only in debug mode) or keypad
byte getButton() {
	byte button = 255;
	byte input;
	// for development, when no keypad is connected to arduino
	if (SERIAL_CONTROL_ACTIVE && Serial.available() > 0) {
		input = Serial.read();
		// Carriage return
		if (input != '\r') {
			DEBUG_PRINTLN("Input: " + String(input));
			button = mapASCIICodeToValue(input);
		}
	} else if (!SERIAL_CONTROL_ACTIVE) {
		byte tmpButton = keypad.readKey();
		// 255 = undefined return of mapASCIICodeToValue
		if (tmpButton != KEY_NOT_PRESSED) {
			button = mapASCIICodeToValue(tmpButton);
		}
	}
	return button;
}

// fills up char with right padding, copied and not changed from https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/
char *rpad(char *dest, const char *str, char chr, unsigned char width) {
	unsigned char len = strlen(str);

	width = width > LCD_COLS ? LCD_COLS : width;

	if (len < LCD_COLS && width > len) {
		strcpy(dest, str);
		strcat(dest, padc(chr, width - len));
	} else {
		strncpy(dest, str, width + 1);
	}
	return dest;
}

// fills up char with left padding, copied and not changed from https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/
char *lpad(char *dest, const char *str, char chr, unsigned char width) {
	unsigned char len = strlen(str);

	width = width > LCD_COLS ? LCD_COLS : width;

	if (len < LCD_COLS && width > len) {
		strcpy(dest, padc(chr, width - len));
		strcat(dest, str);
	} else {
		strncpy(dest, str, width + 1);
	}
	return dest;
}

// fills up char with padding, copied and not changed from https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/
char *padc(char chr, unsigned char count) {
	static char strbuf[LCD_COLS + 1];

	count = (count > LCD_COLS) ? LCD_COLS : count;

	int i;
	for (i = 0; i < count; i++) {
		strbuf[i] = chr;
	}
	strbuf[i] = 0;

	return strbuf;
}

// Returns pot size (small, medium etc.) as String
String getPotSizeString(Potsize potSize) {
	switch (potSize) {
	case pot_size_small:
		return POT_SIZE_STR[0];
		break;
	case pot_size_medium:
		return POT_SIZE_STR[1];
		break;
	case pot_size_large:
		return POT_SIZE_STR[2];
		break;
	default:
		return "undefined";
		break;
	}
}

// Returns moisture (low, medium etc.) as String
String getMoistureString(Moisture moisture) {
	switch (moisture) {
	case moisture_low:
		return POT_MOISTURE_STR[0];
		break;
	case moisture_medium:
		return POT_MOISTURE_STR[1];
		break;
	case moisture_high:
		return POT_MOISTURE_STR[2];
		break;
	default:
		return "undefined";
		break;
	}
}

// Returns moisture (low, medium etc.) as String
int getNeededMoisture(Moisture moisture) {
	switch (moisture) {
	case moisture_low:
		return NEEDED_MOISTURE_LOW;
		break;
	case moisture_medium:
		return NEEDED_MOISTURE_MEDIUM;
		break;
	case moisture_high:
		return NEEDED_MOISTURE_HIGH;
		break;
	default:
		return 0;
		break;
	}
}

// stores selected pot size in potSizeVariable
//TODO could be done better with some kind of index
void togglePotSize(Potsize *potSizeVariable) {
	Potsize *currentSelection = potSizeVariable;
	if (*currentSelection == pot_size_small) {
		*currentSelection = pot_size_medium;
	} else if (*currentSelection == pot_size_medium) {
		*currentSelection = pot_size_large;
	} else if (*currentSelection == pot_size_large) {
		*currentSelection = pot_size_small;
	}
}

// stores selected moisture in moistureVariable
//TODO could be done better with some kind of index
void toggleMoisture(Moisture *moistureVariable) {
	Moisture *currentSelection = moistureVariable;
	if (*currentSelection == moisture_low) {
		*currentSelection = moisture_medium;
	} else if (*currentSelection == moisture_medium) {
		*currentSelection = moisture_high;
	} else if (*currentSelection == moisture_high) {
		*currentSelection = moisture_low;
	}
}


//TODO cases need to be more generic. same things are done multiple times
//----------------------------------------------------------------------
// Addition or removal of menu items in MenuData.h will require this method to be modified accordingly.
byte processMenuCommand(byte cmdId) {
	byte complete = false; // set to true when menu command processing complete.
	//TODO Processing Logs, check Settings etc.
	String currentStateMessage;
	Potsize *currentPotSelection;
	Moisture *currentMoistureSelection;
	switch (cmdId) {
	case menuCommandWaterlevel: {
		int waterLevelPercent = (int) waterLevelSensor.getPercentValue();
		currentStateMessage = String(waterLevelPercent) + "% "
				+ String(multiplyPercent(WATER_TANK_CAPACITY, waterLevelPercent)/ 1000) +
				" " + LITER_STR;
		break;
	}
	case menuCommandPot1Moisture: {
		currentStateMessage = String(soilMoistureSensor1.getPercentValue())	+ "%, ";
		break;
	}
	case menuCommandPot2Moisture: {
		currentStateMessage = String(soilMoistureSensor2.getPercentValue())	+ "%, ";
		break;
	}
	case menuCommandPot1Pump: {
		watering(1, 100);
		complete = true;
		break;
	}
	case menuCommandPot2Pump: {
		watering(2, 100);
		complete = true;
		break;
	}
	case menuCommandPot1Size_Selection: {
		currentPotSelection = &potSize1;
		currentStateMessage = getPotSizeString(*currentPotSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			togglePotSize(currentPotSelection);
		}
		break;
	}
	case menuCommandPot2Size_Selection: {
		currentPotSelection = &potSize2;
		currentStateMessage = getPotSizeString(*currentPotSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			togglePotSize(currentPotSelection);
		}
		break;
	}
	case menuCommandPot1Moisture_Selection: {
		currentMoistureSelection = &neededMoisture1Percent;
		currentStateMessage = getMoistureString(*currentMoistureSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			toggleMoisture(currentMoistureSelection);
		}
		break;
	}
	case menuCommandPot2Moisture_Selection: {
		currentMoistureSelection = &neededMoisture2Percent;
		currentStateMessage = getMoistureString(*currentMoistureSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			toggleMoisture(currentMoistureSelection);
		}
		break;
	}
	default: {
		break;
	}
	}
	// display selection
	showStringMessage(currentStateMessage, 1, 1);
	if (btn == BUTTON_SELECT || btn == BUTTON_BACK) {
		complete = true;
	}
	return complete;
}

//----------------------------------------------------------------------
// Callback to convert button press to navigation action.
byte getNavAction() {
	byte navAction = 0;
	byte currentItemHasChildren = Menu1.currentItemHasChildren();
	if (btn == BUTTON_UP)
		navAction = MENU_ITEM_PREV;
	else if (btn == BUTTON_DOWN)
		navAction = MENU_ITEM_NEXT;
	else if (btn == BUTTON_SELECT
			|| (btn == BUTTON_RIGHT && currentItemHasChildren))
		navAction = MENU_ITEM_SELECT;
	else if (btn == BUTTON_BACK)
		navAction = MENU_BACK;
	return navAction;
}

// Callback to refresh display during menu navigation, using parameter of type enum DisplayRefreshMode.
void refreshMenuDisplay(byte refreshMode) {
	char nameBuf[LCD_COLS + 1];

	lcd.setCursor(0, 0);
	if (Menu1.currentItemHasChildren()) {
		rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
		strbuf[LCD_COLS - 1] = 0b01111110; // Display forward arrow if this menu item has children.
		lcd.print(strbuf);
		lcd.setCursor(0, 1);
		lcd.print(rpad(strbuf, EMPTY_STR));     // Clear config value in display
	} else {
		byte cmdId;
		rpad(strbuf, Menu1.getCurrentItemName(nameBuf));

		if ((cmdId = Menu1.getCurrentItemCmdId()) == 0) {
			strbuf[LCD_COLS - 1] = 0b01111111; // Display back arrow if this menu item ascends to parent.
			lcd.print(strbuf);
			// clear 2nd line
			showStringMessage(EMPTY_STR, 1, 0);
//			lcd.setCursor(0, 1);
//			lcd.print(rpad(strbuf, EMPTY_STR)); // Clear config value in display.
		} else {
			lcd.print(strbuf);
			lcd.setCursor(0, 1);
			lcd.print(" ");
			// clear 2nd line
			showStringMessage(EMPTY_STR, 1, 0);
		}
	}
}
/**************************************************************************************************
 * WATERING FUNCTIONS
 **************************************************************************************************/
//returns pot size in milliliters defined in constants in config.h
int getPotSizeMililiters(Potsize potSize) {
	switch (potSize) {
	case pot_size_small:
		return POT_SIZE_SMALL;
		break;
	case pot_size_medium:
		return POT_SIZE_MEDIUM;
		break;
	case pot_size_large:
		return POT_SIZE_LARGE;
		break;
	default:
		//TODO
		return 10;
	}
}

// returns needed water for watering cycle
int getNeededWaterMilliliters(int pot) {
	Potsize potSize = (pot == 1) ? potSize1 : potSize2;
	// calculate needed water with percentage defined in config.h
	return (getPotSizeMililiters(potSize) / DEFAULT_PERCENTAGE_POT_SIZE);
}

/* returns watering time in milli seconds depending on on
requested quantity, the duty cycle is adapted in order to avoid overflow*/
void watering(int pump, int neededMilliliters) {
	unsigned long int wateringTimeMilliseconds;
	int dutyCycle;
	if (neededMilliliters > 0 && neededMilliliters <= 20) {
		dutyCycle = DUTY_CYCLE_SMALL;
		wateringTimeMilliseconds = ((unsigned long int) (neededMilliliters / PUMP_CAPACITY_SMALL) * 1000);
	}
	else if (neededMilliliters <= 60) {
		dutyCycle = DUTY_CYCLE_MEDIUM;
		wateringTimeMilliseconds = ((unsigned long int) (neededMilliliters / PUMP_CAPACITY_MEDIUM) * 1000);
	}
	else if (neededMilliliters > 60) {
		dutyCycle = DUTY_CYCLE_LARGE;
		wateringTimeMilliseconds = ((unsigned long int) (neededMilliliters / PUMP_CAPACITY_LARGE) * 1000);
	}
	//TODO ugly
	if(wateringTimeMilliseconds > 0 && pump == 1) {
		waterpump1.startWatering(dutyCycle);
		timerPump1.after(wateringTimeMilliseconds, stopPump1);
	}
	else if (wateringTimeMilliseconds > 0) {
		waterpump2.startWatering(dutyCycle);
		timerPump2.after(wateringTimeMilliseconds, stopPump2);
	}
}

// activates the pump for to pump given amount of water into pot
boolean isWaterOK(int neededWaterMilliliters) {
	// check if enough water is in water tank
	if (neededWaterMilliliters
			<= (subtractPercent(WATER_TANK_CAPACITY, MIN_WATER_LEVEL_PERCENT))) {
		return true;
	} else {
		return false;
	}
}

//TODO not really well implemented, but callback functions dont take
// parameters and callback is not supported on instance by library
void stopPump1() {
	waterpump1.stopWatering();
}

void stopPump2() {
	waterpump2.stopWatering();
}

void switchToRunMode() {
	error = false;
	run = true;
	digitalWrite(ERROR_LED_PIN, LOW);
	digitalWrite(RUN_LED_PIN, HIGH);
	//clean up message row
	showStringMessage(EMPTY_STR, 1, 0);
	delay(1000);
}

void switchToErrorMode(String errorMessage) {
	error = true;
	run = false;
	digitalWrite(ERROR_LED_PIN, HIGH);
	digitalWrite(RUN_LED_PIN, LOW);
	showStringMessage(errorMessage, 1, 0);
	delay(1000);
}

void switchOff() {
	error = false;
	run = false;
	digitalWrite(ERROR_LED_PIN, LOW);
	digitalWrite(RUN_LED_PIN, LOW);
	//clean up message row
	showStringMessage(EMPTY_STR, 1, 0);
	delay(1000);
}

void setup() {
	Serial.begin(9600);
	Serial.flush(); // Empty serial puffer
	lcd.clear();
	lcd.begin(LCD_COLS, LCD_ROWS); // set up the LCD's number of columns and rows:
	showDefaultScreen();
	pinMode(RUN_SWITCH_PIN, INPUT);
	pinMode(ERROR_LED_PIN, OUTPUT);
	pinMode(RUN_LED_PIN, OUTPUT);
}

void loop() {
	/**************************************************************************************************
	 * MENU EXECUTION
	 **************************************************************************************************/
	btn = getButton();
	timerPump1.update();
	timerPump2.update();
	switch (appMode) {
	case APP_NORMAL_MODE:
		if (btn == BUTTON_SELECT) {
			// enter menu
			appMode = APP_MENU_MODE;
			refreshMenuDisplay(REFRESH_DESCEND);
		}
//TODO remove
		lcd.setCursor(0, 0);
		break;
	case APP_MENU_MODE: {
		byte menuMode = Menu1.handleNavigation(getNavAction,
				refreshMenuDisplay);

		if (menuMode == MENU_EXIT) {
			// Clean up Display and show default screen
			showDefaultScreen();
			appMode = APP_NORMAL_MODE;
		} else if (menuMode == MENU_INVOKE_ITEM) {
			appMode = APP_PROCESS_MENU_CMD;
			// Indicate selected item.
			lcd.setCursor(0, 1);
			strbuf[0] = 0b01111110; // forward arrow representing input prompt.
			strbuf[1] = 0;
			lcd.print(strbuf);
		}
		break;
	}

	case APP_PROCESS_MENU_CMD: {
		byte processingComplete = processMenuCommand(
				Menu1.getCurrentItemCmdId());

		if (processingComplete) {
			appMode = APP_MENU_MODE;
			// clear 2nd line
			showStringMessage(EMPTY_STR, 1, 0);
		}
		break;
	}
	}
	/**************************************************************************************************
	 * WATER PLANTS
	 **************************************************************************************************/
	onOffSwitch = digitalRead(RUN_SWITCH_PIN);
	if (onOffSwitch == HIGH)
// Switch from ON to OFF and vice versa
	{
		if (!run && !error) {
			switchToRunMode();
			// store start time for logging since we do not have a time server
			startRunTime = millis();
		} else {
			switchOff();
		}
	}
	if (appMode == APP_NORMAL_MODE && !error) {
		int neededWaterMilliliters;
		Serial.println("Run: " + String(run));
		Serial.println("Needed water: " + String(getNeededWaterMilliliters(1)));
		Serial.println("Water ok: " + String(isWaterOK(getNeededWaterMilliliters(1))));
		Serial.println("Needed Moist. 1: " + String(getNeededMoisture(neededMoisture1Percent)));
		Serial.println("Moist. 1: " + String(soilMoistureSensor1.getPercentValue()));
		if (run && soilMoistureSensor1.getPercentValue() < getNeededMoisture(neededMoisture1Percent)) {
			Serial.print("Running");
			neededWaterMilliliters = getNeededWaterMilliliters(1);
			if (isWaterOK(neededWaterMilliliters)) {
				watering(1, neededWaterMilliliters);
				//TODO write log message / maybe show active state over led or display
				//TODO delay and wait for water to seep away
				if (waterpump2.getIsPumpRunning()) {
					showStringMessage(POT_1_WATERING_STR, 1, 0);
				}
				DEBUG_PRINTLN(String(POT_1_WATERING_STR));
			} else {
				/* shutdown, even though there could be enough water for
				 * multiple cycles with plant 2 (smaller pot): plant 1 would die
				 * unnoticed otherwise */
				switchToErrorMode(WATER_LOW_STR);
			}
		}
		if (run && soilMoistureSensor2.getPercentValue() < getNeededMoisture(neededMoisture2Percent)) {
			neededWaterMilliliters = getNeededWaterMilliliters(2);
			if (isWaterOK(neededWaterMilliliters)) {
				watering(2, neededWaterMilliliters);
				//TODO write log message / maybe show active state over led or display
				//TODO delay and wait for water to seep away
				if (waterpump2.getIsPumpRunning()) {
					showStringMessage(POT_2_WATERING_STR, 1, 0);
				}
				DEBUG_PRINTLN(String(POT_2_WATERING_STR));
			} else {
				switchToErrorMode(WATER_LOW_STR);
			}
		}
	}
}

