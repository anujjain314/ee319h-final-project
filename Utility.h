#ifndef _Utility_h
#define _Utility_h
#include <stdint.h>

class Utility{
	public:
		//converts the value to an index that can be used to index into an array of length size
		static uint8_t toIndex(uint32_t val, uint32_t maxVal, uint8_t size);
};

#endif
