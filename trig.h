#ifndef _trig_h
#define _trig_h
#include <stdint.h>

#define NUM_DIRECTIONS 8

	//Cos and Sin Table, Starting at 0 degrees and moving couterclockwise
	const int16_t cos[NUM_DIRECTIONS] = {100,70,0,-70,-100,-70,0,70}; // index = (direction*NUM_DIRECTIONS/255), direction ranges from 0 to 255
	const int16_t sin[NUM_DIRECTIONS] = {0,70,100,70,0,-70,-100,-70};

class Trig{
	public:
		static void setComponents(int16_t*x, int16_t*y, int16_t magnitude, uint8_t direction); // sets x and y components of vector based on magnitude and direction
};

#endif
