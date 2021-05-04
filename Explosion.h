#ifndef _Explosion_h
#define _Explosion_h
#include "Object.h"

#define EXPLOSION_LIFETIME 2 //2 frames before explosion dissapears
#define EXPLOSION_SIZE 1000
#define EXPLOSION_TYPE 4

class Explosion: public Object {
	uint8_t deathCounter;
	virtual const uint8_t* defaultAnimation();
	public:
		Explosion(int16_t x, int16_t y);
		virtual int16_t getType();
};

#endif
