#include "asteroid.h"

const uint8_t* asteroidSpriteList[2] = {AsteroidSmall, AsteroidLarge};

asteroid::asteroid(int16_t x, int16_t y, int16_t vx, int16_t vy, AsteroidType type) : Object(x,y,vx,vy){
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
	if (type == asteroid_small || v.isFull()) return false;
	uint16_t halfSize = ASTEROID_SMALL_SIZE / 2;
	v.push_back(new asteroid(getX(), getY() + halfSize, getDirection(), -getVelocity(), asteroid_small));
	v.push_back(new asteroid(getX() + ASTEROID_SMALL_SIZE, getY() + halfSize, getDirection(), getVelocity(), asteroid_small));
	return true;
}

void asteroid::generateRandomAsteroid(vector<Object*> &v, Ship* player) {
	Random_Init(Random32());
	AsteroidType aType;
	if (Random() < 128) {
		aType = asteroid_small;
	}
	else {
		aType = asteroid_large;
	}
	
	int16_t randX = ((Random32())%12800);
	int16_t randY = ((Random32())%6400);
	int16_t randVX = ((Random32())%3000) - 1500;
	int16_t randVY = ((Random32())%3000) - 1500;
	
//	while (Trig::getDistance(x, player->getX(), y, player->getY()) < 7) {
//		randX = ((Random32()>>24)%128);
//		randY = ((Random32()>>24)%64);
//	}
	
	
	if(Random() < 128){	
		randX += 128;
	} else {
		randX -= 128;
	}
	
	if(Random() < 128){	
		randY += 128;
	} else {
		randY -= 128;
	}
	
	if(randY < 0){
		randVY = ((Random32())%2000) + 1000;
	} else if(randY > MAX_Y){
		randVY = ((Random32())%2000) - 3000;
	}
	
	if(randX < 0){
		randVX = ((Random32())%2000) + 1000;
	} else if(randY > MAX_X){
		randVY = ((Random32())%2000) - 3000;
	}
	
	
	uint8_t dir = Random();
	int16_t velocity = (Random32())%3000;
	
	v.push_back(new asteroid(randX, randY, randVX, randVY, aType));
}

