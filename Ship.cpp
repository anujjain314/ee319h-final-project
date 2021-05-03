#include "Ship.h"

const uint8_t* shipSpriteList[8] = {ShipRight, ShipDownRight, ShipDown, ShipDownLeft, ShipLeft, ShipUpLeft, ShipUp, ShipUpRight};

Ship::Ship(int16_t x, int16_t y) : Object(x,y,0,0){
	maxV = 8000;
	size = SHIP_SIZE;
}

void Ship::fire(Laser &l){
	if(!destroyed)
		l.instantiate(x,y,dir); //instantiate a laser at the ships current position and in the direction the ship is pointing
}

const uint8_t *Ship::defaultAnimation(){
	uint8_t i = Utility::toIndex(dir, 255, 8); //default sprite is based on the direction the ship is moving
	return shipSpriteList[i];
}
