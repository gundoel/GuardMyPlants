/*
 * Waterpump.cpp
 *
 *  Created on: 14.08.2019
 *      Author: TAARISI3
 */

#include <Arduino.h>
#include "Waterpump.h"


Waterpump::Waterpump(byte pin) {
	this->pin = pin;
	pinMode(pin, OUTPUT);
}

void Waterpump::activate(int milliseconds) {
	digitalWrite(pin, HIGH);
	delay(milliseconds);
	digitalWrite(pin, LOW);
}



