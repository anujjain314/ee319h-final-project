#ifndef _Laser_h
#define _Laser_h
#include <stdint.h>
#include "SSD1306.h"
#include "Images.h"
#include "trig.h"
#include "Object.h"

#define LASER_SPEED 300

class Laser : public Object{
	public:
		Laser(int16_t x, int16_t y, uint8_t dir); // initialize with a position and direction, where direction ranges from 0 to 255, 0 is right, 64 is down...
		void instantiate(int16_t x, int16_t y, uint8_t dir); // method to initilize if contructed with default contructor
		Laser();
};

#endif
