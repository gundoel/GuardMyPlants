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
		return (value - (value * percent / 100));
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

#endif
