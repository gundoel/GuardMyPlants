#include "debug.h"
#include <Arduino.h>
#include <IRremote.h>
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

using namespace gmp_string_utils;
using namespace gmp_math_utils;

// Initialize with default values defined in config.h
Potsize potSize1 = DEFAULT_POT_SIZE;
Potsize potSize2 = DEFAULT_POT_SIZE;
Moisture neededMoisture1Percent = DEFAULT_MOISTURE;
Moisture neededMoisture2Percent = DEFAULT_MOISTURE;

// timers to switch off pump after calculated run time
Timer timerPump1;
Timer timerPump2;
int timerId1;
int timerId2;

// millis() when watering activated (for logging)
int startRunTime = 0;

// variables are toggled to handle run and error state
int onOffSwitch = 0;
boolean run = false;
boolean error = false;

// IR receiver
IRrecv irrecv(IR_RECEIVER_PIN);
decode_results results;
unsigned long lastInputRead = 0;

// initialize lcd with interface pins defined in config.h
LiquidCrystal lcd(LCD_PIN1, LCD_PIN2, LCD_PIN3, LCD_PIN4, LCD_PIN5, LCD_PIN6);
char strbuf[LCD_COLS + 1]; // one line of lcd display

/* initialize sensors with interface pins defined in config.h
 * Tested max return value of water level sensor is 320, not 1024 as mentioned in specs.
 * Tested min value is 40. */
WaterLevelSensor waterLevelSensor(0, 336, 10, WATER_LEVEL_SENSOR_PIN, 1);

// soil moisture sensors return value high when moisture low. parameter 1 and 2 switched
SoilMoistureSensor soilMoistureSensor1(1015, 280, 10, SOIL_MOISTURE_SENSOR_1_PIN, 1);
SoilMoistureSensor soilMoistureSensor2(1015, 280, 10, SOIL_MOISTURE_SENSOR_2_PIN, 2);

Waterpump waterpump1(WATERPUMP_1_PIN, 1);
Waterpump waterpump2(WATERPUMP_2_PIN, 2);

// returns button for hex key input of IR receiver
byte mapHexCodeToKey(unsigned long hexCode) {
	switch (hexCode) {
	case 0x511DBB: // VOL+
		return BUTTON_UP;
	case 0xE5CFBD7F: // UP
		return BUTTON_UP;
	case 0x52A3D41F: // FAST BACK
		return BUTTON_BACK;
	case 0xD7E84B1B: // PLAY/PAUSE
		return BUTTON_SELECT;
	case 0x20FE4DBB: // FAST FORWARD
		return BUTTON_RIGHT;
	case 0xA3C8EDDB: // Vol-
		return BUTTON_DOWN;
	case 0xF076C13B: // DOWN
		return BUTTON_DOWN;
	case 0xE318261B: // POWER
		return BUTTON_POWER;
	case 0xFFFFFFFF:
		return BUTTON_REPEAT;
	default:
		return BUTTON_UNUSED;
	}
}

/*returns true if debounce time (config.h) is exceeded
 * in order to debounce key reading without delay() */
boolean isKeyDebounced() {
	if((millis() - DEBOUNCE_TIME) > lastInputRead) {
		lastInputRead = millis();
		return true;
	}
	else {
		return false;
	}
}

// returns button for ascii code input of serial interface
byte mapASCIICodeToKey(byte asciiCode) {
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
	default:
		return BUTTON_UNUSED;
	}
}

/**************************************************************************************************
 * MENU SETUP AND FUNCTIONS
 **************************************************************************************************/
byte btn = BUTTON_UNUSED; // button

enum AppModeValues {
	APP_NORMAL_MODE, APP_MENU_MODE, APP_PROCESS_MENU_CMD
};

/* Apply left padding to string.
https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library */
extern char *lpad(char *dest, const char *str, char chr = ' ',
		unsigned char width = LCD_COLS);

/* Apply right padding to string.
https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library */
extern char *rpad(char *dest, const char *str, char chr = ' ',
		unsigned char width = LCD_COLS);

byte appMode = APP_NORMAL_MODE;
MenuManager Menu1(GMPMenu_Root, menuCount(GMPMenu_Root));

void showDefaultScreen() {
	lcd.clear();
	lcd.print(DEFAULT_SCREEN);
	//reset menu in order to start at root again (e. g. if start button was pressed when in menu)
	Menu1.reset();
}

// clears 2nd line
void clear2ndLine() {
	showStringMessage(EMPTY_STR, 1, 0);
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
	byte button;
	unsigned long input;
	// for development, when no keypad is connected to arduino. no debouncing needed
	if (SERIAL_CONTROL_ACTIVE && Serial.available() > 0) {
		input = Serial.read();
		// Carriage return
		if (input != '\r') {
			DEBUG_PRINTLN("Input: " + String(input));
			button = mapASCIICodeToKey(input);
		}
	}
	// IR receiver
	else if (!SERIAL_CONTROL_ACTIVE && irrecv.decode(&results) && isKeyDebounced()) {
		input = results.value;
		int tmpButton = mapHexCodeToKey(input);
		irrecv.resume();
		if (tmpButton != BUTTON_REPEAT) {
			button = tmpButton;
		}
	}
	else {
		button = BUTTON_REPEAT;
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

// returns true, when processing is completed
//TODO cases need to be more generic. same things are done multiple times
byte processMenuCommand(byte cmdId) {
	byte complete = false; // set to true when menu command processing complete.
	//TODO Logs
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
	case menuCommandReset: {
		showStringMessage(RESET_CONFIRM, 1, 0);
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
		clear2ndLine();
	} else {
		byte cmdId;
		rpad(strbuf, Menu1.getCurrentItemName(nameBuf));

		if ((cmdId = Menu1.getCurrentItemCmdId()) == 0) {
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
	return (getPotSizeMililiters(potSize) / DEFAULT_PERCENTAGE_POT_SIZE);
}

/* returns watering time in milliseconds depending on
requested quantity. duty cycle is adapted in order to avoid overflow */
void watering(int pump, int neededMilliliters) {
	unsigned long wateringTimeMilliseconds;
	int dutyCycle;
	if (neededMilliliters > 0 && neededMilliliters <= 20) {
		dutyCycle = DUTY_CYCLE_SMALL;
		wateringTimeMilliseconds = ((unsigned long) (neededMilliliters / PUMP_CAPACITY_SMALL) * 1000);
	}
	else if (neededMilliliters <= 60) {
		dutyCycle = DUTY_CYCLE_MEDIUM;
		wateringTimeMilliseconds = ((unsigned long) (neededMilliliters / PUMP_CAPACITY_MEDIUM) * 1000);
	}
	else if (neededMilliliters > 60) {
		dutyCycle = DUTY_CYCLE_LARGE;
		wateringTimeMilliseconds = ((unsigned long) (neededMilliliters / PUMP_CAPACITY_LARGE) * 1000);
	}
	//TODO ugly
	if(wateringTimeMilliseconds > 0 && pump == 1) {
		waterpump1.startWatering(dutyCycle);
		// store timer id to stop event when gmp is switched off
		timerId1 = timerPump1.after(wateringTimeMilliseconds, stopPump1);
	}
	else if (wateringTimeMilliseconds > 0 && pump == 2) {
		waterpump2.startWatering(dutyCycle);
		// store timer id to stop event when gmp is switched off
		timerId2 = timerPump2.after(wateringTimeMilliseconds, stopPump2);
	}
}

// returns if enough water for next cycle is in tank
boolean isWaterOK(int neededWaterMilliliters) {
	// check if enough water is in water tank
	if (neededWaterMilliliters
			<= multiplyPercent(subtractPercent(WATER_TANK_CAPACITY, MIN_WATER_LEVEL_PERCENT),
					waterLevelSensor.getPercentValue())) {
		return true;
	} else {
		return false;
	}
}

/*TODO not really well implemented, but callback functions do not take
parameters and callback on instance not supported by library */
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

void switchToRunMode() {
	error = false;
	run = true;
	showDefaultScreen();
	appMode = APP_NORMAL_MODE;
}

void switchToErrorMode(String errorMessage) {
	error = true;
	switchOff();
	showStringMessage(errorMessage, 1, 0);
}

void switchToOffMode() {
	//reset error led
	error = false;
	switchOff();
	clear2ndLine();
}

// stops watering
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
	// reset pumps to allow sartup
	setPump1ToReady();
	setPump2ToReady();
}

void setup() {
	Serial.begin(9600);
	Serial.flush(); // Empty serial puffer

	// clear lcd and set up the LCD's number of columns and rows
	lcd.clear();
	lcd.begin(LCD_COLS, LCD_ROWS);
	showDefaultScreen();

	// set up pin modes
	pinMode(RUN_SWITCH_PIN, INPUT);
	pinMode(ERROR_LED_PIN, OUTPUT);
	pinMode(RUN_LED_PIN, OUTPUT);

	// enable IR receiver
	irrecv.enableIRIn();
}

void loop() {
	/**************************************************************************************************
	 * MENU EXECUTION
	 **************************************************************************************************/
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
	switch (appMode) {
		case APP_NORMAL_MODE:
			if (btn == BUTTON_SELECT) {
				// enter menu
				appMode = APP_MENU_MODE;
				refreshMenuDisplay(REFRESH_DESCEND);
			}
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
				clear2ndLine();
			}
			break;
		}
	}
	/**************************************************************************************************
	 * WATER PLANTS
	 **************************************************************************************************/
	onOffSwitch = digitalRead(RUN_SWITCH_PIN);
	if (onOffSwitch == HIGH || btn == BUTTON_POWER)
// Switch from ON to OFF and vice versa
	{
		if (!run && !error) {
			switchToRunMode();
			// store start time for logging since we do not have a time server
			startRunTime = millis();
		} else {
			switchToOffMode();
		}
	}
	if (appMode == APP_NORMAL_MODE && !error) {
		int neededWaterMilliliters;
//		Serial.println("-------------------");
//		Serial.println("Run: " + String(run));
//		Serial.println("Needed water: " + String(getNeededWaterMilliliters(1)));
//		Serial.println("Water ok: " + String(isWaterOK(getNeededWaterMilliliters(1))));
//		Serial.println("Water level: " + String(waterLevelSensor.getPercentValue()));
//		Serial.println("Needed Moist. 1: " + String(getNeededMoisture(neededMoisture1Percent)));
//		Serial.println("Moist. 1: " + String(soilMoistureSensor1.getPercentValue()));
		if (run && soilMoistureSensor1.getPercentValue() < getNeededMoisturePercent(neededMoisture1Percent)) {
			neededWaterMilliliters = getNeededWaterMilliliters(1);
			if (isWaterOK(neededWaterMilliliters && waterpump1.getIsPumpReady())) {
				//TODO write log message
				if (waterpump1.getIsPumpReady()) {
					watering(1, neededWaterMilliliters);
					showStringMessage(WATERING_STR, 1, 0);
				}
			} else {
				/* shutdown, even though there could be enough water for
				 * multiple cycles with plant 2 (smaller pot): plant 1 would die
				 * unnoticed otherwise */
				switchToErrorMode(WATER_LOW_STR);
			}
		}
		if (run && soilMoistureSensor2.getPercentValue() < getNeededMoisturePercent(neededMoisture2Percent)) {
			neededWaterMilliliters = getNeededWaterMilliliters(2);
			if (isWaterOK(neededWaterMilliliters)) {
				//TODO write log message
				if (waterpump2.getIsPumpReady()) {
					showStringMessage(WATERING_STR, 1, 0);
					watering(2, neededWaterMilliliters);
				}
			} else {
				switchToErrorMode(WATER_LOW_STR);
			}
		}
	}
}

