/*
 * Waterpump.cpp
 *
 * Created on: 07.09.2019
 *
 */

#include "Waterpump.h"
#include "debug.h"

Waterpump::Waterpump(byte pin) {
	this->pin = pin;
	pinMode(pin, OUTPUT);
}

Waterpump::~Waterpump() {}

boolean Waterpump::getIsPumpReady() {
	return isPumpReady;
}

void Waterpump::setIsPumpReady(boolean isPumpReady) {
	this->isPumpReady = isPumpReady;
}

void Waterpump::startWatering(int dutyCycle) {
	analogWrite(pin, dutyCycle);
	setIsPumpReady(false);
}

void Waterpump::stopWatering() {
	digitalWrite(pin, LOW);
}
