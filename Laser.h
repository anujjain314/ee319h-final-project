#ifndef _Laser_h
#define _Laser_h
#include "Object.h"

#define LASER_SPEED 300
#define LASER_LIFETIME 5 // number of frames before the laser should fade away
#define LASER_SIZE 400  // 4 pixels

#define LASER_TYPE 2


class Laser : public Object{
	uint8_t deathCounter;   // laser is destroyed when this reaches 0, has an inital value of LASER_LIFETIME
	virtual const uint8_t* defaultAnimation();
	public:
		Laser(int16_t x, int16_t y , uint8_t dir); // initialize with a position and direction, where direction ranges from 0 to 255, 0 is right, 64 is down...
		Laser(); //create a default, destroyed laser
		void instantiate(int16_t x, int16_t y, uint8_t dir); // method to reinitialize laser
	  virtual int16_t getType();   // returns the Specific type of Object, Ex: SHIP_TYPE, ASTEROID_TYPE, Etc.
};

#endif
