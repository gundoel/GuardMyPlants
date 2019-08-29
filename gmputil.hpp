/*
 * gmputil.h
 *
 *  Created on: 28.07.2019
 *      Author: TAARISI3
 *      Note: File ending *.hpp because it contains implementation. All functions must be marked as "inline" since gmputil.hpp is included in multiple files.
 */

#ifndef GMPUTIL_HPP_
#define GMPUTIL_HPP_

#include <Arduino.h>

namespace gmp_math_utils {

inline double subtractPercent(double value, double percent) {
	return (value - (value * percent));
}

inline double multiplyPercent(double value, double percent) {
	return (value * percent / 100);
}

}

namespace gmp_string_utils {

// Returns the length of str
inline int len(String str) {
	return str.length();
}

}

namespace gmp_keypad_utils {
inline byte mapASCIICodeToValue(byte asciiCode) {
	switch (asciiCode) {
	case 48:
		return 0;
	case 49:
		return 1;
	case 50:
		return 2;
	case 51:
		return 3;
	case 52:
		return 4;
	case 53:
		return 5;
	case 54:
		return 6;
	case 55:
		return 7;
	case 56:
		return 8;
	case 59:
		return 9;
	case 65:
		return 10; //A
	case 66:
		return 11; //B
	case 67:
		return 12; //C
	case 68:
		return 13; //D
	case 97:
		return 10; //a
	case 98:
		return 11; //b
	case 99:
		return 12; //c
	case 100:
		return 13; //d
	case 42:
		return 14; //*
	case 35:
		return 15; //#
	default:
		return 255;
	}
}
}

#endif
