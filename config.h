/*
 * config.h: Bestimmt die Grundeinstellungen wie Pinbelegung und Debug-Modus für Entwicklung
 *
 *  Created on: 14.08.2019
 *      Author: TAARISI3
 */
#define DEBUG // Debug-Modus aktiv, wenn nicht auskommentiert

#ifndef CONFIG_H_
#define CONFIG_H_

/**************************************************************************************************
 * DEBUG
 **************************************************************************************************/

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#endif /* CONFIG_H_ */
