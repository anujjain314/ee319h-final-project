#include "Utility.h"

uint8_t Utility::toIndex(uint32_t val, uint32_t maxVal, uint8_t size){
	return (val*size)/maxVal;
}

int32_t Utility::abs(int32_t x1){
	return x1 >= 0 ? x1 : -x1;
}

int32_t Utility::min(int32_t x, int32_t y){
	return x <= y ? x : y;
}

int32_t Utility::max(int32_t x, int32_t y){
	return x >= y ? x : y;
}
