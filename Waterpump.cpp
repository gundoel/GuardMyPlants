/*
 * Waterpump.cpp
 *
 * Created on: 07.09.2019
 *
 */

#include "Waterpump.h"
#include "debug.h"

Waterpump::Waterpump(byte pin, int id) {
	this->pin = pin;
	this->id = id;
	pinMode(pin, OUTPUT);
}

Waterpump::~Waterpump() {}

int Waterpump::getId() {
	return id;
}

boolean Waterpump::getIsPumpRunning() {
	return isPumpRunning;
}

void Waterpump::setIsPumpRunning(boolean isPumpRunning = false) {
	this->isPumpRunning = isPumpRunning;
}

void Waterpump::startWatering(int dutyCycle) {
	analogWrite(pin, dutyCycle);
	setIsPumpRunning(true);
}

void Waterpump::stopWatering() {
	digitalWrite(pin, LOW);
	setIsPumpRunning(false);
}
