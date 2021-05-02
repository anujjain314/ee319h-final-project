#include "Object.h"

Object::Object(int16_t x, int16_t y, int16_t vx, int16_t vy){
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	maxV = 32767;
	ax = 0;
	ay = 0;
	dir = 192;
}

uint8_t Object::dirToIndex(uint8_t arrLen){
	return (dir*arrLen)/255;
}

uint32_t getMagnitude(int16_t x, int16_t y){
	return x*x+y*y;
}

void Object::move(){
	x = x + vx/20; // update position, assuming this function is called 20 times per second
	y = y + vy/20; 
	
	uint16_t temp1 = vx + ax/20; // calculate velocity
	uint16_t temp2 = vy + ay/20;
	if(getMagnitude(temp1, temp2) < maxV*maxV){ // make sure velocity doesn't exceed max velocity
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

void Object::turn(int16_t speed){
	dir = (dir + speed)&0xFF; // change direction, max is 255;
}

void Object::setAcceleration(int16_t a){
	uint8_t i = dirToIndex(NUM_DIRECTIONS); // caulculate x and y acceleration based on the direction and magnitude
	ax = cos[i] * a; 				                // cos and sin tables defined in trig.h
	ay = sin[i] * a;
}

uint8_t Object::XtoPixels(){
	return x/100;
}

uint8_t Object::YtoPixels(){
	return y/100;
}

void Object::draw(){
	uint8_t i = dirToIndex(numSprites);
	SSD1306_DrawBMP(XtoPixels(),YtoPixels(),sprites[i],0,SSD1306_WHITE);
}
