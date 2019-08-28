#include <Arduino.h>
#include <LiquidCrystal.h>
#include <AnalogMatrixKeypad.h>
#include "MenuData.h"
#include "Sensor.h"
#include "SoilMoistureSensor.h"
#include "WaterLevelSensor.h"
#include "Waterpump.h"
#include "Log.h"
#include "config.h"
#include "gmputil.hpp"

using namespace gmp_keypad_utils;
using namespace gmp_string_utils;
using namespace gmp_math_utils;

// Initialize with default values defined in config.h
Potsize potSize1 = DEFAULT_POT_SIZE;
Potsize potSize2 = DEFAULT_POT_SIZE;
Moisture neededMoisture1Percent = DEFAULT_MOISTURE;
Moisture neededMoisture2Percent = DEFAULT_MOISTURE;

int onOffSwitch = 0;
boolean run = false;
char strbuf[LCD_COLS + 1]; // one line of lcd display

// initialize lcd with interface pins defined in config.h
LiquidCrystal lcd(LCD_PIN1, LCD_PIN2, LCD_PIN3, LCD_PIN4, LCD_PIN5, LCD_PIN6);

// initialize keypad with interface pin defined in config.h
AnalogMatrixKeypad keypad(ANALOG_KEYPAD_PIN);

/* initialize sensors with interface pins defined in config.h
Tested max return value of water level sensor is 320, not 1024 as mentioned in specs
 */
WaterLevelSensor waterLevelSensor(0, 320, WATER_LEVEL_SENSOR_PIN,
		MIN_WATER_LEVEL_PERCENT);
SoilMoistureSensor soilMoistureSensor1(0, 320, SOIL_MOISTURE_SENSOR_1_PIN,
		neededMoisture1Percent);
SoilMoistureSensor soilMoistureSensor2(0, 320, SOIL_MOISTURE_SENSOR_2_PIN,
		neededMoisture2Percent);
Waterpump waterpump1(WATERPUMP_1_PIN);
Waterpump waterpump2(WATERPUMP_2_PIN);

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

/**************************************************************************************************
 * MENU SETUP AN FUNCTIONS
 **************************************************************************************************/
byte btn; // button

enum AppModeValues {
	APP_NORMAL_MODE, APP_MENU_MODE, APP_PROCESS_MENU_CMD, APP_TOGGLE_SELECTION
};

//TODO move functions to some header
void refreshMenuDisplay(byte refreshMode);
byte getNavAction();

byte appMode = APP_NORMAL_MODE;
MenuManager Menu1(GMPMenu_Root, menuCount(GMPMenu_Root));

//TODO move functions to some header file
void showDefaultScreen() {
	lcd.print(DEFAULT_SCREEN);
}

void showStateMessage(String message) {
	//State messages always on 2nd line
	lcd.setCursor(0,1);
	// TODO scrolling for longer messages
//	if(len(message) > LCD_COLS) {
//		lcd.autoscroll()
//	}
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
		button = keypad.readKey();
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
String getPotSizeString(int potSize) {
	switch (potSize) {
	case pot_size_small:
		return POT_SIZE_STRING[0];
		break;
	case pot_size_medium:
		return POT_SIZE_STRING[1];
		break;
	case pot_size_large:
		return POT_SIZE_STRING[2];
		break;
	default:
		return "undefined";
		break;
	}
}

// Returns moisture (low, medium etc.) as String
String getMoistureString(int moisture) {
	switch (moisture) {
	case moisture_low:
		return POT_MOISTURE_STRING[0];
		break;
	case moisture_medium:
		return POT_MOISTURE_STRING[1];
		break;
	case moisture_high:
		return POT_MOISTURE_STRING[2];
		break;
	default:
		return "undefined";
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
// Toggles selection items
byte toggleSelection(byte cmdId) {
	byte complete = false;
	// show selection on line 2 an leave out 1 character for arrow
	lcd.setCursor(1, 1);
	char buf[LCD_COLS];
	String currentSelectionString;
	Potsize *currentPotSelection;
	Moisture *currentMoistureSelection;
	switch (cmdId) {
	case menuCommandPot1Size_Selection: {
		currentPotSelection = &potSize1;
		currentSelectionString = getPotSizeString(*currentPotSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			togglePotSize(currentPotSelection);
		}
		break;
	}
	case menuCommandPot2Size_Selection: {
		currentPotSelection = &potSize2;
		currentSelectionString = getPotSizeString(*currentPotSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			togglePotSize(currentPotSelection);
		}
		break;
	}
	case menuCommandPot1Moisture_Selection: {
		currentMoistureSelection = &neededMoisture1Percent;
		currentSelectionString = getMoistureString(*currentMoistureSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			toggleMoisture(currentMoistureSelection);
		}
		break;
	}
	case menuCommandPot2Moisture_Selection: {
		currentMoistureSelection = &neededMoisture2Percent;
		currentSelectionString = getMoistureString(*currentMoistureSelection);
		// toggle selection when button right pressed
		if (btn == BUTTON_RIGHT) {
			toggleMoisture(currentMoistureSelection);
		}
		break;
	}
	}
	// display selection
	currentSelectionString.toCharArray(buf, len(currentSelectionString + 1));
	lcd.print(rpad(strbuf, buf));
	if (btn == BUTTON_BACK || btn == BUTTON_SELECT) {
		complete = true;
	}
	return complete;
}

//----------------------------------------------------------------------
// Addition or removal of menu items in MenuData.h will require this method to be modified accordingly.
byte processMenuCommand(byte cmdId) {
	byte complete = false; // set to true when menu command processing complete.

	if (btn == BUTTON_SELECT) {
		complete = true;
	}

	//TODO Processing Logs, check Settings etc.
	switch (cmdId) {
	default:
		break;
	}

	return complete;
}

//----------------------------------------------------------------------
// Callback to convert button press to navigation action.
byte getNavAction() {
	byte navAction = 0;
	byte currentItemHasChildren = Menu1.currentItemHasChildren();
	// current item never has selection if it has children
	byte currentItemHasSelection =
			(currentItemHasChildren) ? false : Menu1.currentItemHasSelection();
	if (btn == BUTTON_UP)
		navAction = MENU_ITEM_PREV;
	else if (btn == BUTTON_DOWN)
		navAction = MENU_ITEM_NEXT;
	else if (btn == BUTTON_SELECT
			|| (btn == BUTTON_RIGHT && currentItemHasChildren))
		navAction = MENU_ITEM_SELECT;
	else if (btn == BUTTON_RIGHT && currentItemHasSelection)
		navAction = MENU_TOGGLE_SELECTION;
	else if (btn == BUTTON_BACK)
		navAction = MENU_BACK;
	return navAction;
}

// Callback to refresh display during menu navigation, using parameter of type enum DisplayRefreshMode.
void refreshMenuDisplay(byte refreshMode) {
	char nameBuf[LCD_COLS + 1];

	/*
	 if (refreshMode == REFRESH_DESCEND || refreshMode == REFRESH_ASCEND)
	 {
	 byte menuCount = Menu1.getMenuItemCount();

	 // uncomment below code to output menus to serial monitor
	 if (Menu1.currentMenuHasParent())
	 {
	 Serial.print("Parent menu: ");
	 Serial.println(Menu1.getParentItemName(nameBuf));
	 }
	 else
	 {
	 Serial.println("Main menu:");
	 }

	 for (int i=0; i<menuCount; i++)
	 {
	 Serial.print(Menu1.getItemName(nameBuf, i));

	 if (Menu1.itemHasChildren(i))
	 {
	 Serial.println("->");
	 }
	 else
	 {
	 Serial.println();
	 }
	 }
	 }
	 */

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
			lcd.setCursor(0, 1);
			lcd.print(rpad(strbuf, EMPTY_STR)); // Clear config value in display.
		} else {
			lcd.print(strbuf);
			lcd.setCursor(0, 1);
			lcd.print(" ");
			lcd.print(rpad(strbuf, EMPTY_STR)); // Clear config value in display.
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

double getPumpTimeMilliSeconds(int neededWaterMilliliters) {
	return (neededWaterMilliliters / PUMP_MILLILITERS_PER_SECOND);
}

// activates the pump for the calculated amount of seconds
boolean waterPlant(int pot) {
	//TODO not really beautiful
	Potsize potSize = (pot == 1) ? potSize1 : potSize2;
	// calculate needed water with percentage defined in config.h
	int neededWaterMilliliters = getPotSizeMililiters(potSize)
			/ DEFAULT_PERCENTAGE_POT_SIZE;
	// check if enough water is in water tank
	if (neededWaterMilliliters
			>= (subtractPercent(WATER_TANK_CAPACITY,
					waterLevelSensor.getThresholdValuePercent()))) {
		if (pot == 1) {
			waterpump1.activate(
					getPumpTimeMilliSeconds(neededWaterMilliliters));
		} else if (pot == 2) {
			waterpump2.activate(neededWaterMilliliters);
		}
		return true;
	} else {
		return false;
	}
}

void loop() {
	/**************************************************************************************************
	 * MENU EXECUTION
	 **************************************************************************************************/
	btn = getButton();
	if (btn != 255) {
		DEBUG_PRINTLN("Button: " + String(btn));
	}
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
			DEBUG_PRINTLN("Menumode MENU_EXIT");
			// Clean up Display and show default screen
			lcd.clear();
			showDefaultScreen();
			appMode = APP_NORMAL_MODE;
		} else if (menuMode == MENU_SELECTION) {
			DEBUG_PRINTLN("Selection mode");
			appMode = APP_TOGGLE_SELECTION;
		} else if (menuMode == MENU_INVOKE_ITEM) {
			appMode = APP_PROCESS_MENU_CMD;
			Serial.print("Invoke");
			// Indicate selected item.
			lcd.setCursor(0, 1);
			strbuf[0] = 0b01111110; // forward arrow representing input prompt.
			strbuf[1] = 0;
			lcd.print(strbuf);
		}
		break;
	}
	case APP_TOGGLE_SELECTION: {
		byte togglingComplete = toggleSelection(Menu1.getCurrentItemCmdId());
// Indicate selected item.
		lcd.setCursor(0, 1);
		strbuf[0] = 0b01111110; // forward arrow representing input prompt.
		strbuf[1] = 0;
		lcd.print(strbuf);
		if (togglingComplete) {
			appMode = APP_MENU_MODE;
			// clear forward arrow
			lcd.setCursor(0, 1);
			strbuf[0] = ' '; // clear forward arrow
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
			// clear forward arrow
			lcd.setCursor(0, 1);
			strbuf[0] = ' '; // clear forward arrow
			strbuf[1] = 0;
			lcd.print(strbuf);
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
		delay(50); // Debounce
		if (!run) {
			run = true;
			digitalWrite(RUN_LED_PIN, HIGH);

		} else {
			run = false;
			digitalWrite(RUN_LED_PIN, LOW);
		}
	}
// check if GuardMyPlants is switched ON
	//TODO Threads?
	if (run) {
		if (soilMoistureSensor1.getPercentValue() < neededMoisture1Percent) {
			if (waterPlant(1)) {
				//TODO write log message / maybe show active state over led or display
				//TODO delay and wait for water to seep away
				showStateMessage(PLANT_1_WATERED);
			}
			else {
				showStateMessage(PLANT_1_WATER_LOW);
			}
		}
		if (soilMoistureSensor2.getPercentValue() < neededMoisture2Percent) {
			if (waterPlant(2)) {
				//TODO write log message / maybe show active state over led or display
				//TODO delay and wait for water to seep away
				showStateMessage(PLANT_2_WATERED);
			}
			else {
				showStateMessage(PLANT_2_WATER_LOW);
			}
		}
	}
}

