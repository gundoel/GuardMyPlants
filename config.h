/*
 * config.h defines basic settings like pins, messages and debug mode
 *
 *  Created on: 14.08.2019
 *      Author: TAARISI3
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Arduino.h"

/**************************************************************************************************
 * DEVELOPMENT
 **************************************************************************************************/
// also consider changing preprocessor DEBUG in debug.h when changing this parameter
const boolean SERIAL_CONTROL_ACTIVE = true; // allows control of GuardMyPlants over serial input if not commented out

/**************************************************************************************************
 * PUMP & WATER TANK
 **************************************************************************************************/
// todo pumpenklasse braucht eine bewaesserungsmethode, der nur die benoetigte menge
// in ml uebergeben werden kann. foerdermenge und pwm interessieren GMP nicht, die
// pumpenklasse muss das handeln
// defines flow rate of pump in milliliters
const int PUMP_MILLILITERS_PER_SECOND = 100;
const int PUMP_PWM = 70;
// defines how many per cent of pot size are pumped into the pot in 1 cycle. e. g. pot = 500ml -> 50ml
const int DEFAULT_PERCENTAGE_POT_SIZE = 10;
// defines capacity of water tank in milliliters
// TODO measure full tank and set value here
const int WATER_TANK_CAPACITY = 1000;
// defines a minimum for water level to prevent pumps from sucking in air an breaking
const int MIN_WATER_LEVEL_PERCENT = 10;


/**************************************************************************************************
 * POTS
 **************************************************************************************************/
// potsize values in milliliters
const int POT_SIZE_SMALL = 200;
const int POT_SIZE_MEDIUM = 1000;
const int POT_SIZE_LARGE = 2000;

enum Potsize {
	pot_size_small, pot_size_medium, pot_size_large
};
const Potsize DEFAULT_POT_SIZE = pot_size_medium;

// needed moisture percent values
const int NEEDED_MOISTURE_LOW = 10;
const int NEEDED_MOISTURE_MEDIUM = 25;
const int NEEDED_MOISTURE_HIGH = 50;
enum Moisture {moisture_low, moisture_medium, moisture_high};
const Moisture DEFAULT_MOISTURE = moisture_medium;


/**************************************************************************************************
 * UI TEXTS
 **************************************************************************************************/
// available pot sizes. sequence must correspond with enum Potsize
const String POT_SIZE_STR[] { "klein", "mittel", "gross" };
// minimal moisture in a pot (%). sequence must correspond with enum Needed_Moisture
const String POT_MOISTURE_STR[] { "leicht feucht", "mittelfeucht", "feucht" };
const String POT_1_WATERED_STR = "Topf 1 bewaess.";
const String POT_2_WATERED_STR = "Topf 2 bewaess.";
const String WATER_LOW_STR = "Tank fuellen!";
const String LITER_STR = "Liter";

/**************************************************************************************************
 * DISPLAY & MENU
 **************************************************************************************************/
#define LCD_ROWS 2
#define LCD_COLS 16
const char EMPTY_STR[] = ""; // Empty String for padding etc. used in menus
enum {
//	LCD_PIN1 = 4,
//	LCD_PIN2 = 6,
//	LCD_PIN3 = 7,
//	LCD_PIN4 = 8,
//	LCD_PIN5 = 9,
//	LCD_PIN6 = 10
	LCD_PIN1 = 8,
	LCD_PIN2 = 9,
	LCD_PIN3 = 10,
	LCD_PIN4 = 11,
	LCD_PIN5 = 12,
	LCD_PIN6 = 13
};

// defines which of the menus in MenuData.h are selection menus
// TODO not really beautiful
const unsigned char SELECTION_MENUS[] {2, 3, 4, 5};

#define DEFAULT_SCREEN "GuardMyPlants"

/**************************************************************************************************
 * PINS
 **************************************************************************************************/
const byte WATER_LEVEL_SENSOR_PIN = A0;
const byte SOIL_MOISTURE_SENSOR_1_PIN = A1;
const byte SOIL_MOISTURE_SENSOR_2_PIN = A2;
const byte ANALOG_KEYPAD_PIN = 4;

// PWM Pins
const byte WATERPUMP_1_PIN = 3;
const byte WATERPUMP_2_PIN = 5;

const byte RUN_LED_PIN = 6;
const byte ERROR_LED_PIN = 7;
const byte RUN_SWITCH_PIN = 4;

/**************************************************************************************************
 * BUTTONS
 **************************************************************************************************/
// button numbers must correspond with return values of gmp_keypad_utils method in gmputil
enum {
	BUTTON_UP = 10, //A
	BUTTON_DOWN = 11, //B
	BUTTON_RIGHT = 12, //C
	BUTTON_BACK = 13, //D
	BUTTON_SELECT = 15 //#
};

#endif /* CONFIG_H_ */
