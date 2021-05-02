#include "Laser.h"

const uint8_t* laserSpriteList[8] = {HorizontalLaser, DownDiagonalLaser, VerticalLaser, UpDiagonalLaser, HorizontalLaser, DownDiagonalLaser, VerticalLaser, UpDiagonalLaser};

Laser::Laser(int16_t x, int16_t y, uint8_t dir) : Object(x,y,0,0){
	instantiate(x,y,dir);
}

void Laser::instantiate(int16_t x, int16_t y, uint8_t dir){
	this->x = x;
	this->y = y;
	this->dir = dir;
	uint8_t i = dirToIndex(NUM_DIRECTIONS);
	vx = cos[i] * LASER_SPEED;
	vy = sin[i] * LASER_SPEED;
	numSprites = 8;
	sprites = laserSpriteList;
}

Laser::Laser() : Object(0,0,0,0){
}
