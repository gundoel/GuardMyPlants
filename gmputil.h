/*
 * WaterLevelSensor.h
 *
 *  Created on: 28.07.2019
 *      Author: TAARISI3
 */

#ifndef GMPUTIL_H_
#define GMPUTIL_H_

namespace gmputil {
	double calculatePercentValue(int min = 0, int max = 100, int value = 0) {
		return (double) value * 100 / (max - min);
	}
}

#endif
