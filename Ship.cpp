#include "Ship.h"


//accleration values for different directions when the magnitude of acceleration is 1
const int16_t x_acc[NUM_DIRECTIONS] = {100,70,0,-70,-100,-70,0,70}; // direction is the index
const int16_t y_acc[NUM_DIRECTIONS] = {0,70,100,70,0,-70,-100,-70};

//sprites
const uint8_t* sprites[NUM_DIRECTIONS] = {ShipRight, ShipDownRight, ShipDown, ShipDownLeft, ShipLeft, ShipUpLeft, ShipUp, ShipUpRight};

Ship::Ship(int16_t x, int16_t y){
	this->x = x;
	this->y = y;
	vx = 0;
	vy = 0;
	ax = 0;
	ay = 0;
	dir = 192;
}

uint8_t Ship::getIndex(){
	return (dir*NUM_DIRECTIONS)/255;
}

uint32_t getMagnitude(int16_t x, int16_t y){
	return x*x+y*y;
}

void Ship::Move(){
	x = x + vx/20; // update position, assuming this function is called 20 times per second
	y = y + vy/20; 
	
	uint16_t temp1 = vx + ax/20; // calculate velocity
	uint16_t temp2 = vy + ay/20;
	if(getMagnitude(temp1, temp2) < MAX_V*MAX_V){ // make sure velocity doesn't exceed max velocity
		vx = temp1; //update velocity
		vy = temp2;
	}
	
	if(x > MAX_X)  // wrapping
		x = MIN_X;
	if(y > MAX_Y)
		y = MIN_Y;
	if(x < MIN_X)
		x = MAX_X;
	if(y < MIN_Y)
		y = MAX_Y;
}

void Ship::turn(int16_t speed){
	dir = (dir + speed)&0xFF; // change direction, max is 255;
}

void Ship::setAcceleration(int16_t a){
	uint8_t i = getIndex(); // caulculate x and y acceleration based on the direction and magnitude
	ax = x_acc[i] * a;
	ay = y_acc[i] * a;
}

uint8_t Ship::XtoPixels(){
	return x/100;
}

uint8_t Ship::YtoPixels(){
	return y/100;
}

void Ship::draw(){
	uint8_t i = getIndex();
	SSD1306_DrawBMP(XtoPixels(),YtoPixels(),sprites[i],0,SSD1306_WHITE);
}
