/*
 * LogMessage.h
 *
 *  Created on: 25.08.2019
 *      Author: TAARISI3
 */

#ifndef LOGMESSAGE_H_
#define LOGMESSAGE_H_

#include <Arduino.h>

class LogMessage {

private:
	String timestamp;
	String message;

public:
	LogMessage(String timestamp, String message);
	~LogMessage();
	String getMessage();
};

#endif /* LOGMESSAGE_H_ */
