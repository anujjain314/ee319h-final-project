#ifndef _Ship_h
#define _Ship_h
#include "Object.h"
#include "Laser.h"

#define SHIP_SIZE 800

//sprites
//const uint8_t* const sprites[NUM_SPRITES] = {ShipRight, ShipDownRight, ShipDown, ShipDownLeft, ShipLeft, ShipUpLeft, ShipUp, ShipUpRight};

class Ship : public Object{
	const uint8_t* defaultAnimation();
	public:
		Ship(int16_t x, int16_t y);
		void fire(Laser &l);
};

#endif

