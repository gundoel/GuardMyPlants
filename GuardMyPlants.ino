/*
 * GuardMyPlants.ino
 *
 *  Created on: 28.07.2019
 *  Author: GMP Team
 *  Version: 1.0
 *  Main Arduino Sketch. Libraries and additional files included here.
 */


#include "debug.h"
#include <Arduino.h>
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Timer.h>
#include "config.h"
#include "gmputils.hpp"
#include "MenuData.h"
#include "Sensor.h"
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"

using namespace gmp_string_utils;
using namespace gmp_math_utils;

/**************************************************************************************************
 * GLOBAL VARIABLES AND OBJECTS
 **************************************************************************************************/

// one line of lcd display
char strbuf[LCD_COLS + 1];

// initialize lcd with interface pins defined in config.h
LiquidCrystal lcd(lcdPins[0], lcdPins[1], lcdPins[2], lcdPins[3], lcdPins[4], lcdPins[5]);

// timers to switch off pump after calculated run time
Timer timerPump1;
Timer timerPump2;
int timerId1;
int timerId2;

/* initialize sensors with interface pins defined in config.h
 * Tested max return value of water level sensor is 320, not 1024 as mentioned in specs.
 * Tested min value is 30. */
WaterLevelSensor waterLevelSensor(40, 360, 20, WATER_LEVEL_SENSOR_PIN);

// soil moisture sensors return value high when moisture low. parameters 1 and 2 switched
SoilMoistureSensor soilMoistureSensor1(1015, 280, 10, SOIL_MOISTURE_SENSOR_1_PIN);
SoilMoistureSensor soilMoistureSensor2(1015, 280, 10, SOIL_MOISTURE_SENSOR_2_PIN);

Waterpump waterpump1(WATERPUMP_1_PIN);
Waterpump waterpump2(WATERPUMP_2_PIN);

// IR receiver
IRrecv irReceiver(IR_RECEIVER_PIN);
decode_results results;
// milliseconds when last input was read (used for debouncing)
unsigned long lastInputRead = 0;

// defines used buttons (input values are mapped to these buttons)
enum Buttons {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_BACK,
	BUTTON_SELECT,
	BUTTON_UNUSED,
	BUTTON_POWER,
	BUTTON_REPEAT
};

// states of menu
enum AppModeValues {
	APP_NORMAL_MODE, APP_MENU_MODE, APP_PROCESS_MENU_CMD
};

// button used to control menu interaction and starting watering
byte btn = BUTTON_UNUSED;
byte appMode = APP_NORMAL_MODE;
MenuManager GMPMenu(GMPMenu_Root, menuCount(GMPMenu_Root));

// Initialize with default values defined in config.h
Potsize potSize1 = DEFAULT_POT_SIZE;
Potsize potSize2 = DEFAULT_POT_SIZE;
Moisture neededMoisture1Percent = DEFAULT_MOISTURE;
Moisture neededMoisture2Percent = DEFAULT_MOISTURE;

// variables are toggled at runtime to handle run and error state
boolean run = false;
boolean error = false;

/**************************************************************************************************
 * DISPLAY FUNCTIONS
 **************************************************************************************************/
// fills up char with right padding, copied and not changed from https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/
char *rpad(char *dest, const char *str, char chr = ' ', unsigned char width = LCD_COLS) {
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
char *lpad(char *dest, const char *str, char chr = ' ',	unsigned char width = LCD_COLS) {
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

// shows default welcome screen
void showDefaultScreen() {
	lcd.clear();
	lcd.print(DEFAULT_SCREEN);
	/*reset menu in order to start at root again
	 * (e. g. if start button was pressed when in menu)*/
	GMPMenu.reset();
}

// clears 2nd line of display
void clear2ndLine() {
	showStringMessage(EMPTY_STR, 1, 0);
}

/* displays given message on given line / row of display
note: max. length of messages is LCD_COLS, scrolling not yet implemented  */
void showStringMessage(String message, int line, int offset) {
	lcd.setCursor(offset, line);
	char buf[LCD_COLS];
	// TODO scrolling for longer messages
	message.toCharArray(buf, len(message) + 1);
	lcd.print(rpad(strbuf, buf));
	lcd.print(message);
}

/**************************************************************************************************
 * USER INPUT FUNCTIONS (KEYBOARD WHEN DEBUGGING OR IR RECEIVER)
 **************************************************************************************************/

// returns button for hex key input of IR receiver
byte mapHexCodeToKey(unsigned long hexCode) {
	Serial.println(String(hexCode));
	switch (hexCode) {
	case 0x511DBB:
		return BUTTON_UP; // VOL+
	case 0xE5CFBD7F:
		return BUTTON_UP;// UP
	case 0x52A3D41F:
		return BUTTON_BACK; // FAST BACK
	case 16712445:
		return BUTTON_SELECT;
	case 0xD7E84B1B:
		return BUTTON_SELECT; // PLAY/PAUSE
	case 16761405:
		return BUTTON_RIGHT;
	case 0x20FE4DBB:
		return BUTTON_RIGHT; // FAST FORWARD
	case 16754775:
		return BUTTON_DOWN;
	case 0xA3C8EDDB:
		return BUTTON_DOWN; // Vol-
	case 0xF076C13B:
		return BUTTON_DOWN; // DOWN
	case 0xE318261B:
		return BUTTON_POWER; // POWER
	case 0xFFFFFFFF:
		return BUTTON_REPEAT;
	default:
		return BUTTON_UNUSED;
	}
}

// returns button for ascii code input of serial interface
byte mapASCIICodeToKey(byte asciiCode) {
	Serial.println(String(asciiCode));
	switch (asciiCode) {
	case 87:
		return BUTTON_UP; // W = BUTTON_UP
	case 88:
		return BUTTON_DOWN; // X = BUTTON_DOWN
	case 68:
		return BUTTON_RIGHT; // D = BUTTON_RIGHT
	case 65:
		return BUTTON_BACK; // A = BUTTON_BACK
	case 83:
		return BUTTON_SELECT; // S = BUTTON_SELECT
	case 82:
		return BUTTON_POWER; // R = BUTTON_POWER
	case 119:
		return BUTTON_UP; // w = BUTTON_UP
	case 120:
		return BUTTON_DOWN; // x = BUTTON_DOWN
	case 100:
		return BUTTON_RIGHT; // d = BUTTON_RIGHT
	case 97:
		return BUTTON_BACK; // a = BUTTON_BACK
	case 115:
		return BUTTON_SELECT; // s = BUTTON_SELECT
	case 114:
		return BUTTON_POWER; // r = BUTTON_POWER
	default:
		return BUTTON_UNUSED;
	}
}

/* returns true if debounce time (see config.h) is exceeded
 in order to debounce key reading without delay() */
boolean isKeyDebounced() {
	if((millis() - DEBOUNCE_TIME) > lastInputRead) {
		lastInputRead = millis();
		return true;
	}
	else {
		return false;
	}
}

// returns button either from Serial (only in debug mode) or IR receiver
byte getButton() {
	byte button;
	unsigned long input;
	// for development, when no keypad is connected to arduino
	// TODO menu works only if key is debounced -> does not make sense
	if (SERIAL_CONTROL_ACTIVE && Serial.available() > 0 && isKeyDebounced()) {
		input = Serial.read();
		// Carriage return
		if (input != '\r') {
			DEBUG_PRINTLN("Input: " + String(input));
			button = mapASCIICodeToKey(input);
		}
	}
	// IR receiver
	else if (!SERIAL_CONTROL_ACTIVE && irReceiver.decode(&results) && isKeyDebounced()) {
		input = results.value;
		int tmpButton = mapHexCodeToKey(input);
		irReceiver.resume();
		if (tmpButton != BUTTON_REPEAT) {
			button = tmpButton;
		}
	}
	else {
		button = BUTTON_REPEAT;
	}
	return button;
}

/**************************************************************************************************
 * MENU FUNCTIONS
 **************************************************************************************************/

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

// stores selected pot size in potSizeVariable
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

/* processes command selected in menu. only called in appMode APP_PROCESS_MENU_CMD.
returns true, when processing is completed
TODO cases could be more generic. same things are done multiple times*/
byte processMenuCommand(byte cmdId) {
	byte complete = false; // set to true when menu command processing complete.
	String currentStateMessage;
	Potsize *currentPotSelection;
	Moisture *currentMoistureSelection;
	switch (cmdId) {
	// indicates current water level in tank
	case menuCommandWaterlevel: {
		int waterLevelPercent = (int) waterLevelSensor.getPercentValue();
		currentStateMessage = String(waterLevelPercent) + "% "
				+ String(multiplyPercent(WATER_TANK_CAPACITY_ML, waterLevelPercent)/ 1000) +
				" " + LITER_STR;
		break;
	}
	// indicates current moisture in pot 1
	case menuCommandPot1Moisture: {
		currentStateMessage = String(soilMoistureSensor1.getPercentValue())	+ "%, ";
		break;
	}
	// indicates current moisture in pot 2
	case menuCommandPot2Moisture: {
		currentStateMessage = String(soilMoistureSensor2.getPercentValue())	+ "%, ";
		break;
	}
	// pumps amount of water defined in config.h int pot 1
	case menuCommandPot1Pump: {
		doWaterPlants(1, TEST_MODE_WATER_ML);
		complete = true;
		break;
	}
	// pumps amount of water defined in config.h int pot 2
	case menuCommandPot2Pump: {
		//showStringMessage(String(TEST_MODE_WATER_ML) + MILLILITER_STR, 2, 1);
		//showStringMessage("hans", 1, 1);
		doWaterPlants(2, TEST_MODE_WATER_ML);
		complete = true;
		break;
	}
	// toggles pot size for pot 1. selection is stored immediately (no action needed)
	case menuCommandPot1Size_Selection: {
		currentPotSelection = &potSize1;
		currentStateMessage = getPotSizeString(*currentPotSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			togglePotSize(currentPotSelection);
		}
		break;
	}
	// toggles pot size for pot 2. selection is stored immediately (no action needed)
	case menuCommandPot2Size_Selection: {
		currentPotSelection = &potSize2;
		currentStateMessage = getPotSizeString(*currentPotSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			togglePotSize(currentPotSelection);
		}
		break;
	}
	// toggles needed moisture for pot 1. selection is stored immediately (no action needed)
	case menuCommandPot1Moisture_Selection: {
		currentMoistureSelection = &neededMoisture1Percent;
		currentStateMessage = getMoistureString(*currentMoistureSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			toggleMoisture(currentMoistureSelection);
		}
		break;
	}
	// toggles needed moisture for pot 2. selection is stored immediately (no action needed)
	case menuCommandPot2Moisture_Selection: {
		currentMoistureSelection = &neededMoisture2Percent;
		currentStateMessage = getMoistureString(*currentMoistureSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			toggleMoisture(currentMoistureSelection);
		}
		break;
	}
	// resets pot size and moisture to default values defined in config.h
	case menuCommandReset: {
		currentStateMessage = RESET_CONFIRM;
		//showStringMessage(RESET_CONFIRM, 1, 0);
		if (btn == BUTTON_SELECT) {
			neededMoisture1Percent = DEFAULT_MOISTURE;
			neededMoisture2Percent = DEFAULT_MOISTURE;
			potSize1 = DEFAULT_POT_SIZE;
			potSize2 = DEFAULT_POT_SIZE;
			clear2ndLine();
			complete = true;
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

/* Callback to convert button press to navigation action.
 * copied but changed from https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/ */
byte getNavAction() {
	byte navAction = 0;
	byte currentItemHasChildren = GMPMenu.currentItemHasChildren();
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

/* Callback to refresh display during menu navigation, using parameter of type enum DisplayRefreshMode.
 * copied but changed from https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/ */
void refreshMenuDisplay(byte refreshMode) {
	char nameBuf[LCD_COLS + 1];
	lcd.setCursor(0, 0);
	if (GMPMenu.currentItemHasChildren()) {
		rpad(strbuf, GMPMenu.getCurrentItemName(nameBuf));
		strbuf[LCD_COLS - 1] = 0b01111110; // Display forward arrow if this menu item has children.
		lcd.print(strbuf);
		clear2ndLine();
	} else {
		byte cmdId;
		rpad(strbuf, GMPMenu.getCurrentItemName(nameBuf));

		if ((cmdId = GMPMenu.getCurrentItemCmdId()) == 0) {
			strbuf[LCD_COLS - 1] = 0b01111111; // Display back arrow if this menu item ascends to parent.
			lcd.print(strbuf);
			// clear 2nd line
			showStringMessage(EMPTY_STR, 1, 0);
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
// Returns moisture (low, medium etc.) as percent value
int getNeededMoisturePercent(Moisture moisture) {
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
		return POT_SIZE_SMALL;
	}
}

// returns needed water for watering cycle
int getNeededWaterMilliliters(int pot) {
	Potsize potSize = (pot == 1) ? potSize1 : potSize2;
	// calculate needed water with percentage defined in config.h
	return multiplyPercent(getPotSizeMililiters(potSize), DEFAULT_PERCENTAGE_POT_SIZE);
}

/* returns watering time in milliseconds depending on
requested quantity. duty cycle is adapted in order to avoid overflow */
void doWaterPlants(int pumpNr, int neededMilliliters) {
	double wateringTimeMilliseconds;
	int dutyCycle;
	if (neededMilliliters > 0 && neededMilliliters <= 20) {
		dutyCycle = DUTY_CYCLE_SMALL;
		DEBUG_PRINTLN("Duty cycle small");
		wateringTimeMilliseconds = ((neededMilliliters / PUMP_CAPACITY_SMALL) * 1000);
	}
	else if (neededMilliliters <= 60) {
		dutyCycle = DUTY_CYCLE_MEDIUM;
		DEBUG_PRINTLN("Duty cycle medium");
		wateringTimeMilliseconds = ((neededMilliliters / PUMP_CAPACITY_MEDIUM) * 1000);
	}
	else if (neededMilliliters > 60) {
		dutyCycle = DUTY_CYCLE_LARGE;
		DEBUG_PRINTLN("Duty cycle large");
		wateringTimeMilliseconds = ((neededMilliliters / PUMP_CAPACITY_LARGE) * 1000);
	}
	if(wateringTimeMilliseconds > 0 && pumpNr == 1) {
		waterpump1.startWatering(dutyCycle);
		// store timer id to stop event when gmp is switched off
		timerId1 = timerPump1.after(wateringTimeMilliseconds, stopPump1);
	}
	else if (wateringTimeMilliseconds > 0 && pumpNr == 2) {
		waterpump2.startWatering(dutyCycle);
		// store timer id to stop event when gmp is switched off
		timerId2 = timerPump2.after(wateringTimeMilliseconds, stopPump2);
	}
}

// returns if enough water for next cycle is in tank
boolean isWaterOKForCurrentCycle(int neededWaterMilliliters) {
	// check if enough water is in water tank
	if (neededWaterMilliliters
			<= multiplyPercent(subtractPercent(WATER_TANK_CAPACITY_ML, MIN_WATER_LEVEL_PERCENT),
					waterLevelSensor.getPercentValue())) {
		return true;
	} else {
		return false;
	}
}

/* wrapper functions, since callback functions do not take
parameters and callback on object instance not supported by timer library */
void setPump1ToReady() {
	waterpump1.setIsPumpReady(true);
}

void setPump2ToReady() {
	waterpump2.setIsPumpReady(true);
}

void stopPump1() {
	waterpump1.stopWatering();
	// start seep away timer
	timerId1 = timerPump1.after(SEEP_AWAY_TIME_MS, setPump1ToReady);
	clear2ndLine();
}

void stopPump2() {
	waterpump2.stopWatering();
	// start seep away timer
	timerId2 = timerPump2.after(SEEP_AWAY_TIME_MS, setPump2ToReady);
	clear2ndLine();
}

// starts watering program
void switchToRunMode() {
	error = false;
	run = true;
	showDefaultScreen();
	appMode = APP_NORMAL_MODE;
}

// resets error (in case of error) and stops watering program
void switchToOffMode() {
	//reset error led
	error = false;
	switchOff();
	clear2ndLine();
}

// changes state to error and stops watering program
void switchToErrorMode(String errorMessage) {
	error = true;
	switchOff();
	showStringMessage(errorMessage, 1, 0);
}

// stops watering program
void switchOff() {
	run = false;
	// stop pumps
	waterpump1.stopWatering();
	waterpump2.stopWatering();
	// cancel events
	timerPump1.stop(timerId1);
	timerPump2.stop(timerId2);
	digitalWrite(RUN_LED_PIN, LOW);
	digitalWrite(ERROR_LED_PIN, LOW);
	// reset pumps to allow startup
	setPump1ToReady();
	setPump2ToReady();
}

/**************************************************************************************************
 * ARDUINO MAIN FUNCTIONS
 **************************************************************************************************/
void setup() {
	Serial.begin(9600);
	Serial.flush(); // Empty serial puffer

	// clear lcd and set up the LCD's number of columns and rows
	lcd.clear();
	lcd.begin(LCD_COLS, LCD_ROWS);
	showDefaultScreen();

	// set up pin modes
	pinMode(ERROR_LED_PIN, OUTPUT);
	pinMode(RUN_LED_PIN, OUTPUT);

	// enable IR receiver
	irReceiver.enableIRIn();
}

void loop() {
	btn = getButton();
	// update all timers
	timerPump1.update();
	timerPump2.update();

	// indicate state with leds
	if(run) {
		digitalWrite(ERROR_LED_PIN, LOW);
		digitalWrite(RUN_LED_PIN, HIGH);
	}
	else if(error) {
		digitalWrite(ERROR_LED_PIN, HIGH);
		digitalWrite(RUN_LED_PIN, LOW);
	}
/**************************************************************************************************
 * MENU EXECUTION
 **************************************************************************************************/
	switch (appMode) {
		case APP_NORMAL_MODE:
			if (btn == BUTTON_SELECT) {
				// enter menu
				appMode = APP_MENU_MODE;
				refreshMenuDisplay(REFRESH_DESCEND);
			}
			break;
		case APP_MENU_MODE: {
			byte menuMode = GMPMenu.handleNavigation(getNavAction,
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
					GMPMenu.getCurrentItemCmdId());

			if (processingComplete) {
				appMode = APP_MENU_MODE;
				clear2ndLine();
			}
			break;
		}
	}
/**************************************************************************************************
 * WATERING
 **************************************************************************************************/
	if (btn == BUTTON_POWER)
	// Switch from ON to OFF and vice versa
	{
		if (!run && !error) {
			switchToRunMode();
		} else {
			switchToOffMode();
		}
	}
	/* check if water level is higher than minimum for pump
	 * always check when running, not only when plants would need water
	 * to notice immediately when water is low
	 */
	if(run && waterLevelSensor.getPercentValue() <= MIN_WATER_LEVEL_PERCENT) {
		switchToErrorMode(WATER_LOW_STR);
	}
	// watering is continued while in menu. no check for APP_NORMAL_MODE
	if (!error) {
		int neededWaterMilliliters;
		// pump is running or water is seeping away, if pump is not ready
		if(run && waterpump1.getIsPumpReady()) {
			// watering necessary?
			if (soilMoistureSensor1.getPercentValue() < getNeededMoisturePercent(neededMoisture1Percent)) {
				neededWaterMilliliters = getNeededWaterMilliliters(1);
				// enough water for current cycle?
				if (isWaterOKForCurrentCycle(neededWaterMilliliters)) {
					doWaterPlants(1, neededWaterMilliliters);
					// supress Message when in Menu
					if(appMode == APP_NORMAL_MODE) {
						showStringMessage(WATERING_STR, 1, 0);
					}
				} else {
					/* shutdown, even though there could be enough water for
					 * multiple cycles with plant 2 (smaller pot): plant 1 would die
					 * unnoticed otherwise */
					switchToErrorMode(WATER_LOW_STR);
				}
			}
		}
		// pump is running or water is seeping away, if pump is not ready
		if(run && waterpump2.getIsPumpReady()) {
			// watering necessary?
			if (soilMoistureSensor2.getPercentValue() < getNeededMoisturePercent(neededMoisture2Percent)) {
				neededWaterMilliliters = getNeededWaterMilliliters(2);
				// enough water for current cycle?
				if (isWaterOKForCurrentCycle(neededWaterMilliliters)) {
					doWaterPlants(2, neededWaterMilliliters);
					// supress Message when in Menu
					if(appMode == APP_NORMAL_MODE) {
						showStringMessage(WATERING_STR, 1, 0);
					}
				} else {
					switchToErrorMode(WATER_LOW_STR);
				}
			}
		}
	}
}
