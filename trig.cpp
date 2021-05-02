#include "trig.h"
#include "Utility.h"

void Trig::setComponents(int16_t*x, int16_t*y, int16_t magnitude, uint8_t direction){
	uint8_t i = Utility::toIndex(direction, 255, NUM_DIRECTIONS); // convert direction to an index that can be used to index into sin/cos arrays
	*x = cos[i] * magnitude;
	*y = sin[i] * magnitude;
}
