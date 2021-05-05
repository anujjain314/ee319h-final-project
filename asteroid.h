#ifndef _asteroid_h
#define _asteroid_h
#include "Object.h"
#include "Ship.h"
#include "random.h"
#include "../inc/tm4c123gh6pm.h"

#define ASTEROID_SMALL_SIZE 800
#define ASTEROID_LARGE_SIZE 1800

#define ASTEROID_TYPE 1

enum AsteroidType {asteroid_small, asteroid_large};

class asteroid : public Object{
	AsteroidType type;
	virtual const uint8_t* defaultAnimation();
	public:
		asteroid(int16_t x = 0, int16_t y = 0, uint8_t dir = 0, int16_t velocity = 0, AsteroidType type = asteroid_small);
	  virtual int16_t getType();   // returns the Specific type of Object, Ex: SHIP_TYPE, ASTEROID_TYPE, Etc.
		bool breakDown(vector<Object*> &v);
		static void generateRandomAsteroid(vector<Object*> &v, Ship* player);
};

#endif
