/*
 * Waterpump.h
 *
 *  Created on: 14.08.2019
 *      Author: TAARISI3
 */

#ifndef WATERPUMP_H_
#define WATERPUMP_H_

class Waterpump {
private:
	byte pin;

public:
	Waterpump(byte pin);
	void activate(int milliseconds);
};

#endif /* WATERPUMP_H_ */
