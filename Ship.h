#ifndef _Ship_h
#define _Ship_h
#include <stdint.h>
#include "SSD1306.h"
#include "Images.h"

#define NUM_DIRECTIONS 8
// Min and Max values for position and velocity
#define MAX_X 12000
#define MAX_Y 5800
#define MIN_X 800
#define MIN_Y 800
#define MAX_V 8000

class Ship {
	//fixed point, divide by 100 to get number of pixels
	int16_t x;	//x-position - ranges from 0 to 12,800
	int16_t y;  //y-position - ranges from 0 to 6400
	int16_t vx; //x-velocity  - ranges from 0 to 1600
	int16_t vy; //y-velocity  - ranges from 0 to 1600
	int16_t ax; //x-acceleration
	int16_t ay; //y-acceleration
	//direction - Ranges from 0 to 255, where 0 is right, 64 is Down, 128 is left, 192 is up
	uint16_t dir;
	uint8_t getIndex(); // returns the index associated with the current direction, used to index into sprite and acceleration value arrays
	uint8_t XtoPixels(); // returns the pixel value associated with the x_pos
	uint8_t YtoPixels(); // returns the pixel value associated with the y_pos
	public:
		Ship(int16_t x, int16_t y);
		void Move(); 	// changes the x and y positions according to the velocity, changes velocity accoring to the acceleration
		void turn(int16_t speed); //changes the direction by an amount proportional to speed
		void setAcceleration(int16_t a); // sets the acceleration in the current direction (units of a is pixels/s^2)
		void draw(); // draws the ship on the SSD_1306
};

#endif

