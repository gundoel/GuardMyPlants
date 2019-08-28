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

class Log {
public:
	Log();

	const void addMessage(LogMessage *message);
	const LogMessage getMessage(int index);

private:
	LogMessage messages[100];
	int count = 0;
};

#endif /* LOG_H_ */
