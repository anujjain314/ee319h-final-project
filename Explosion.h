#ifndef _Explosion_h
#define _Explosion_h
#include "Object.h"

#define EXPLOSION_LIFETIME 4 //2 frames before explosion dissapears
#define EXPLOSION_SIZE 1000
#define EXPLOSION_TYPE 4

class Explosion: public Object {
	uint8_t deathCounter;
	const uint8_t* defaultAnimation();
	public:
		Explosion(int x, int y);
		int16_t getType();
};

#endif
