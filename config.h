/*
 * config.h: Bestimmt die Grundeinstellungen wie Pinbelegung und Debug-Modus für Entwicklung
 *
 *  Created on: 14.08.2019
 *      Author: TAARISI3
 */
#define DEBUG // Debug-Modus aktiv, wenn nicht auskommentiert
#include "Arduino.h"

#ifndef CONFIG_H_
#define CONFIG_H_

/**************************************************************************************************
 * PORTS
 **************************************************************************************************/

//TODO Datentyp in uint8_t aendern
const int WATER_LEVEL_SENSOR_PIN = A0;
const int SOIL_MOISTURE_SENSOR_1_PIN = A1;
const int SOIL_MOISTURE_SENSOR_2_PIN = A2;
const int WATERPUMP_1_PIN = A3;
const int WATERPUMP_2_PIN = A4;
//const int ANALOG_KEYPAD_PIN = A5;
const int LCD_PINS[] = {5, 6, 7, 8, 9, 10};
const int RUN_LED_PIN = 11;
const int ERROR_LED_PIN = 12;
const int RUN_SWITCH_PIN = 13;

/**************************************************************************************************
 * DEBUG
 **************************************************************************************************/

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_GET_SERIAL_INPUT() getSerialInput()
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_GET_SERIAL_INPUT()
#endif

String getSerialInput();

#endif /* CONFIG_H_ */
