#include "Ship.h"

const uint8_t* shipSpriteList[8] = {ShipRight, ShipDownRight, ShipDown, ShipDownLeft, ShipLeft, ShipUpLeft, ShipUp, ShipUpRight};

Ship::Ship(int16_t x, int16_t y) : Object(x,y,0,0){
	numSprites = 8;
	maxV = 8000;
	this->sprites = shipSpriteList;
}

void Ship::fire(Laser &l){
	l.instantiate(x,y,dir);
}
