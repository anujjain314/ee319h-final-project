#include "trig.h"
#include "Utility.h"

void Trig::setComponents(int16_t*x, int16_t*y, int16_t magnitude, uint8_t direction){
	uint8_t i = Utility::toIndex(direction, 255, NUM_DIRECTIONS); // convert direction to an index that can be used to index into sin/cos arrays
	*x = cos[i] * magnitude;
	*y = sin[i] * magnitude;
}

int32_t Trig::getDistance(int32_t x1, int32_t x2, int32_t y1, int32_t y2){
	return Utility::max( Utility::abs(x1 - x2), Utility::abs(y1 - y2) ); // distance calculated as the largest difference between components
}
