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
// allows control of GuardMyPlants over serial input if not commented out
// also consider changing preprocessor DEBUG in debug.h when changing this parameter
const boolean SERIAL_CONTROL_ACTIVE = false;

/**************************************************************************************************
 * PUMP & WATER TANK
 **************************************************************************************************/
// defines pwm values for different flow rates
const int DUTY_CYCLE_SMALL = 55; // Duty Cycle for small amounts
const int DUTY_CYCLE_MEDIUM = 100; // Duty Cycle for medium amounts
const int DUTY_CYCLE_LARGE = 150; // Duty Cycle for large amounts

// defines flow rate of pump in milliliters
const float PUMP_CAPACITY_SMALL = 6; // 6 ml/s
const float PUMP_CAPACITY_MEDIUM = 19; // 19 ml/s
const float PUMP_CAPACITY_LARGE = 23; // 23 ml/s

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
// minimum potsize values in milliliters
// edit POT_SIZE_STR if edited
const int POT_SIZE_SMALL = 100;
const int POT_SIZE_MEDIUM = 500;
const int POT_SIZE_LARGE = 1000;

// time in milliseconds water needs to seep away before watering again
const unsigned long SEEP_AWAY_TIME_MS = 5000;

enum Potsize {
	pot_size_small, pot_size_medium, pot_size_large
};
/* default must be small to prevent overflow: if pot is bigger,
watering is executed more often in smaller amounts (no problem) */
const Potsize DEFAULT_POT_SIZE = pot_size_small;

// needed moisture percent values
const int NEEDED_MOISTURE_LOW = 30;
const int NEEDED_MOISTURE_MEDIUM = 50;
const int NEEDED_MOISTURE_HIGH = 70;
enum Moisture {moisture_low, moisture_medium, moisture_high};
const Moisture DEFAULT_MOISTURE = moisture_medium;

/**************************************************************************************************
 * UI TEXTS
 **************************************************************************************************/
const String DEFAULT_SCREEN = "GuardMyPlants";
const String RESET_CONFIRM = "Sicher?";

// available pot sizes. sequence must correspond with enum Potsize
const String POT_SIZE_STR[] { "klein >= 100ml", "mittel >= 500ml", "gross >=1000ml" };

// minimal moisture in a pot (%). sequence must correspond with enum Moisture
const String POT_MOISTURE_STR[] { "leicht feucht", "mittelfeucht", "feucht" };
const String WATERING_STR = "Bewaesserung";
const String WATER_LOW_STR = "Tank fuellen!";
const String LITER_STR = "Liter";

/**************************************************************************************************
 * DISPLAY & MENU
 **************************************************************************************************/
#define LCD_ROWS 2
#define LCD_COLS 16

// Empty String for padding etc. used in menus
const char EMPTY_STR[] = "";
enum {
	LCD_PIN1 = 8,
	LCD_PIN2 = 9,
	LCD_PIN3 = 10,
	LCD_PIN4 = 11,
	LCD_PIN5 = 12,
	LCD_PIN6 = 13
};

/**************************************************************************************************
 * PINS
 **************************************************************************************************/
const byte WATER_LEVEL_SENSOR_PIN = A0;
const byte SOIL_MOISTURE_SENSOR_1_PIN = A1;
const byte SOIL_MOISTURE_SENSOR_2_PIN = A2;
const byte IR_RECEIVER_PIN = 2;

// PWM Pins
const byte WATERPUMP_1_PIN = 5;
const byte WATERPUMP_2_PIN = 6;

const byte RUN_LED_PIN = 3;
const byte ERROR_LED_PIN = 4;

/**************************************************************************************************
 * BUTTONS
 **************************************************************************************************/
// defines used buttons (input values are mapped to these buttons)
enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_BACK,
	BUTTON_SELECT,
	BUTTON_UNUSED,
	BUTTON_POWER,
	BUTTON_REPEAT
};

const unsigned long DEBOUNCE_TIME = 500;

#endif /* CONFIG_H_ */
