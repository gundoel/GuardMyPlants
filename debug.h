/*
 * debug.h: debugging print function used in multiple files
 *
 *  Created on: 09.09.2019
 *  Author: GMP Team
 *  Version: 1.0
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/**************************************************************************************************
 * DEBUG
 **************************************************************************************************/
//#define DEBUG // debug mode active, if not commented out
#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#endif /* DEBUG_H_ */
