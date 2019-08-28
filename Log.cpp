/*
 * Log.cpp
 *
 *  Created on: 25.08.2019
 *      Author: TAARISI3
 */

#include "Log.h"

const void Log::addMessage(LogMessage *message) {
	//TODO delete Log needed (allocation of memory with new)
//	LogMessage* messages = new LogMessage[100];
	count++;
}

const LogMessage Log::getMessage(int index) {
	return messages[index];
}


