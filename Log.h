/*
 * Log.h
 *
 *  Created on: 25.08.2019
 *      Author: TAARISI3
 */

#ifndef LOG_H_
#define LOG_H_

#include <Arduino.h>
#include "LogMessage.h"
#include "debug.h"

class Log {
public:
//	Log();

	void addMessage(LogMessage *message);
	const LogMessage getMessage(int index);
	void printLogEntries();
	int getLastStoredIndex();

private:
	//store the last 20 log messages
	LogMessage messages[20];
	int lastStoredIndex = 0;
};

#endif /* LOG_H_ */
