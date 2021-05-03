#include "Laser.h"

const uint8_t* laserSpriteList[8] = {HorizontalLaser, DownDiagonalLaser, VerticalLaser, UpDiagonalLaser, HorizontalLaser, DownDiagonalLaser, VerticalLaser, UpDiagonalLaser};

Laser::Laser(int16_t x, int16_t y, uint8_t dir) : Object(x,y,0,0){
	instantiate(x,y,dir);
}

void Laser::instantiate(int16_t x, int16_t y, uint8_t dir){
	this->x = x;
	this->y = y;
	this->dir = dir;
	size = LASER_SIZE;
	deathCounter = LASER_LIFETIME;
	Trig::setComponents(&vx,&vy, LASER_SPEED, dir);
}

const uint8_t* Laser::defaultAnimation(){
	// destroy the object after LASER_LIMETIME calls to this function
	deathCounter--;
	if(deathCounter == 0){
		destroy();
		return NULL; // destroyed object shouldnt be visible
	}
	
	uint8_t i = Utility::toIndex(dir, 255, 8); //default sprite is based on the direction the laser is moving
	return laserSpriteList[i];
}
