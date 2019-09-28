/*
 * gmputil.h
 *
 *  Created on: 28.07.2019
 *  Author: GMP Team
 *  Version: 1.0
 *      Note: File ending *.hpp because it contains implementation.
 *      All functions must be marked as "inline" since gmpfunctions.hpp
 *      is included in multiple files.
 */

#ifndef GMPUTILS_HPP_
#define GMPUTILS_HPP_

#include <Arduino.h>

namespace gmp_math_utils {

	// subtracts percent from value and returns result
	inline double subtractPercent(double value, double percent) {
		return (value - (value * percent / 100));
	}

	// multiplies percent from value and returns result
	inline double multiplyPercent(double value, double percent) {
		return (value * percent / 100);
	}

}

// Returns the length of str
namespace gmp_string_utils {

	inline int len(String str) {
		return str.length();
	}
}

#endif
