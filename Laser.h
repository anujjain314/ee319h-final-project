#ifndef _Laser_h
#define _Laser_h
#include "Object.h"

#define LASER_SPEED 300
#define LASER_LIFETIME 5 // number of frames before the laser should fade away
#define LASER_SIZE 400  // 4 pixels

class Laser : public Object{
	uint8_t deathCounter;   // laser is destroyed when this reaches 0, has an inital value of LASER_LIFETIME
	const uint8_t* defaultAnimation();
	public:
		Laser(int16_t x = 0, int16_t y = 0, uint8_t dir = 0); // initialize with a position and direction, where direction ranges from 0 to 255, 0 is right, 64 is down...
		void instantiate(int16_t x, int16_t y, uint8_t dir); // method to initilize if contructed with default contructor
};

#endif
