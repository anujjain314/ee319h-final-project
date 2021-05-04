#include "Explosion.h"

#define EXPLOSION_NUM_SPRITES 2

const uint8_t* explosionSpriteList[EXPLOSION_NUM_SPRITES] = {BigExplosion0, BigExplosion1};

Explosion::Explosion(int x, int y) : Object(x, y, 0, 0){
	deathCounter = EXPLOSION_LIFETIME;
	size = EXPLOSION_SIZE;
}

const uint8_t* Explosion::defaultAnimation(){
	deathCounter--;
	if(deathCounter == 0){
		destroy();
		return NULL; // destroyed object shouldnt be visible
	}
	return explosionSpriteList[deathCounter % EXPLOSION_NUM_SPRITES];
}


int16_t Explosion::getType(){
	return EXPLOSION_SIZE;
}
