#ifndef _Ship_h
#define _Ship_h
#include <stdint.h>
#include "SSD1306.h"
#include "Images.h"
#include "trig.h"
#include "Object.h"
#include "Laser.h"

// Min and Max values for position and velocity
#define MAX_X 12000
#define MAX_Y 5600
#define MIN_X 800
#define MIN_Y 800
#define MAX_V 8000

//sprites
//const uint8_t* const sprites[NUM_SPRITES] = {ShipRight, ShipDownRight, ShipDown, ShipDownLeft, ShipLeft, ShipUpLeft, ShipUp, ShipUpRight};

class Ship : public Object{
	const uint8_t* defaultAnimation();
	public:
		Ship(int16_t x, int16_t y);
		void fire(Laser &l);
};

#endif

