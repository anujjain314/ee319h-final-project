#ifndef _Utility_h
#define _Utility_h
#include <stdint.h>


class Utility{
	
	public:
		//converts the value to an index that can be used to index into an array of length size
		static uint8_t toIndex(uint32_t val, uint32_t maxVal, uint8_t size);
		static int32_t abs(int32_t x); // returns the absolute value of x
	  static int32_t min(int32_t x, int32_t y); //returns the smaller of the two inputs
		static int32_t max(int32_t x, int32_t y); //returns the larger of the two inputs
	  static int32_t getSign(int32_t x); // returns the sign of the number: 1, -1, or 0
	  static int32_t sameSign(int32_t x, int32_t y);
};

#endif
