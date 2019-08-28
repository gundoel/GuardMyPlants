/*
 * LogMessage.cpp
 *
 *  Created on: 25.08.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "LogMessage.h"

LogMessage::LogMessage(String timestamp, String message) {
	this->timestamp = timestamp;
	this->message = message;
}

