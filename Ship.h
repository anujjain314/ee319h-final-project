#ifndef _Ship_h
#define _Ship_h
#include "Object.h"
#include "Laser.h"

#define SHIP_SIZE 800

#define SHIP_TYPE 3


class Ship : public Object{
	virtual const uint8_t* defaultAnimation();
	public:
		Ship(int16_t x, int16_t y);
		void fire(Laser &l);
	  virtual int16_t getType();   // returns the Specific type of Object, Ex: SHIP_TYPE, ASTEROID_TYPE, Etc.
};

#endif

