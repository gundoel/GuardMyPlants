/*
 * WaterLevelSensor.h
 *
 *  Created on: 28.07.2019
 *      Author: TAARISI3
 *      Note: File ending *.hpp because it contains implementation. All functions must be marked as "inline" since gmputil.hpp is included in multiple files.
 */

#ifndef GMPUTIL_HPP_
#define GMPUTIL_HPP_

#include <Arduino.h>
#include <map>

namespace gmp_math_utils {
	inline double calculatePercentValue(int min = 0, int max = 100, int value = 0) {
		return (double) value * 100 / (max - min);
	}
}

namespace gmp_string_utils {
	// Source: https://create.arduino.cc/projecthub/ianabcumming/a-simple-arduino-menu-with-an-lcd-254080
	inline String mid(String str, int start, int len) {
	   int t = 0;
	   String u = "";
	   for (t = 0; t < len;t++) {
	    u = u + str.charAt(t+start-1);
	   }
	   return u;
	}

	inline int inStrRev(String str,String chr) {
	  int t = str.length()-1;
	  int u = 0;
	   while (t>-1) {
	    if (str.charAt(t)==chr.charAt(0)) {
	      u = t+1;t = -1;
	    }
	    t = t - 1;
	   }
	  return u;
	}

	inline int len(String str) {
	  return str.length();
	}
}

namespace gmp_keypad_utils {
	inline byte mapASCIICodeToValue(byte asciCode) {
		return 1;
	}

	// Creates a Map, which maps ASCII codes to key numbers. Returns the reference to the map.
	inline std::map<byte,byte> buildASCIIMap() {
		std::map<byte,byte> ASCIIMap;
		ASCIIMap[48]=0;
		ASCIIMap[49]=1;
		ASCIIMap[50]=2;
		ASCIIMap[51]=3;
		ASCIIMap[52]=4;
		ASCIIMap[53]=5;
		ASCIIMap[54]=6;
		ASCIIMap[55]=7;
		ASCIIMap[56]=8;
		ASCIIMap[59]=9;
		ASCIIMap[65]=10; //A
		ASCIIMap[66]=11; //B
		ASCIIMap[67]=12; //C
		ASCIIMap[68]=13; //D
		ASCIIMap[97]=10; //a
		ASCIIMap[98]=11; //b
		ASCIIMap[99]=12; //c
		ASCIIMap[100]=13; //d
		ASCIIMap[42]=14; //*
		ASCIIMap[35]=15; //#
		return ASCIIMap;
	}
}

#endif
