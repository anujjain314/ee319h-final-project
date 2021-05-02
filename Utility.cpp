#include "Utility.h"

uint8_t Utility::toIndex(uint32_t val, uint32_t maxVal, uint8_t size){
	return (val*size)/maxVal;
}