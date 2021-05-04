#include "asteroid.h"

const uint8_t* asteroidSpriteList[2] = {AsteroidSmall, AsteroidLarge};

asteroid::asteroid(int16_t x, int16_t y, uint8_t dir, int16_t velocity, AsteroidType type) : Object(x,y,0,0){
	Trig::setComponents(&vx, &vy, velocity, dir);
	this->type = type;
	if(type == asteroid_small)
		size = ASTEROID_SMALL_SIZE;
	else
		size = ASTEROID_LARGE_SIZE;
}

const uint8_t* asteroid::defaultAnimation(){
	if(type == asteroid_small)
		return asteroidSpriteList[0];
	else
		return asteroidSpriteList[1];
}

int16_t asteroid::getType(){
	return ASTEROID_TYPE;
}

bool asteroid::breakDown(vector<Object*> &v) {
	if (type == asteroid_small) return false;
	uint16_t halfSize = ASTEROID_SMALL_SIZE / 2;
	v.push_back(new asteroid(getX(), getY() + halfSize, getDirection(), -getVelocity(), asteroid_small));
	v.push_back(new asteroid(getX() + ASTEROID_SMALL_SIZE, getY() + halfSize, getDirection(), getVelocity(), asteroid_small));
	return true;
}
