/*
 * Waterpump.h
 *
 *  Created on: 14.08.2019
 *      Author: TAARISI3
 */

#ifndef WATERPUMP_H_
#define WATERPUMP_H_

class Waterpump {
	public:
		Waterpump(int analogPin);
		void activate(int milliseconds);
};


#endif /* WATERPUMP_H_ */
