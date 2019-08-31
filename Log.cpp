/*
 * Log.cpp
 *
 *  Created on: 25.08.2019
 *      Author: TAARISI3
 */

#include "Log.h"

void Log::addMessage(LogMessage *message) {
	// delete current message to prevent memory leak
	//TODO check if necessary
	delete &messages[lastStoredIndex];
	messages[lastStoredIndex] = *message;
	lastStoredIndex++;
	// start at the beginning. shifting or sorting array would be too expensive
	if (lastStoredIndex == sizeof(messages) -1) {
		// back to 0
		lastStoredIndex = 0;
	}
}

const LogMessage Log::getMessage(int index) {
	return messages[index];
}

//TODO test circular printing
void Log::printLogEntries() {
	int i = lastStoredIndex;
	do {
		if(messages[i].getMessage() != "") {
			//TODO print to display
			DEBUG_PRINTLN(messages[i].getMessage());
		}
		i--;
		if(i == -1) {
			//start at the end until lastStoredIndex is reached
			i = sizeof(messages) -1;
		}
	} while(i != lastStoredIndex);
}

int Log::getLastStoredIndex() {
	return lastStoredIndex;
}


