#include "Object.h"

Object::Object(int16_t x, int16_t y, int16_t vx, int16_t vy){
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	size = 1000; // default size 10 pixels;
	animation = NULL;
	maxV = 32767;
	ax = 0;
	ay = 0;
	dir = 192;
	destroyed = false;
}


uint32_t getMagnitude(int16_t x, int16_t y){
	return x*x+y*y;
}

void Object::move(){
	x = x + vx/FRAME_RATE;
	y = y + vy/FRAME_RATE; 
	
	uint16_t temp1 = vx + ax/FRAME_RATE; // calculate velocity
	uint16_t temp2 = vy + ay/FRAME_RATE;
	if(getMagnitude(temp1, temp2) < maxV*maxV){ // make sure velocity doesn't exceed max velocity
		vx = temp1; //update velocity
		vy = temp2;
	}
	
	if(x > MAX_X - size)  // wrapping
		x = 0;
	if(y > MAX_Y)
		y = size;
	if(x < 0)
		x = MAX_X - size;
	if(y < size)
		y = MAX_Y;
}

void Object::turn(int16_t speed){
	dir = (dir + speed)&0xFF; // change direction, max is 255;
}

void Object::setAcceleration(int16_t a){ // caulculate x and y acceleration based on the direction and magnitude
	Trig::setComponents(&ax, &ay, a, dir);
}

uint8_t Object::XtoPixels(){
	return x/100;
}

uint8_t Object::YtoPixels(){
	return y/100;
}

bool Object::isDestoyed(){
	return destroyed;
}

void Object::destroy(){
	destroyed = true;
}

int16_t Object::getX(){
	return x;
}

int16_t Object::getY(){
	return y;
}

int16_t Object::getSize(){
	return size;
}

bool Object::isColliding(Object &other){
	int16_t xCenterOther = other.getX() + other.getSize()/2; 	//get x and y position of the center of both objects
	int16_t yCenterOther = other.getY() - other.getSize()/2;
	int16_t xCenterThis = this->x + this->size/2;
	int16_t yCenterThis = this->y - this->size/2;
	
	int16_t distance = Trig::getDistance(xCenterOther, xCenterThis, yCenterOther, yCenterThis); //get distances between centers
	
	return (distance < (other.getSize()/2 + this->size/2)); // collision if distance between centers is less than half the sum of the objects' sizes
}

void Object::getCollisions(vector<Object> &others, vector<Object> &collisions){
	for(uint8_t i = 0; i < others.len(); i++){
		if(isColliding(others[i]))
			collisions.push_back(others[i]);
	}
}

void Object::setAnimation(const uint8_t **anim, uint8_t fps, uint8_t numFrames){
	animation = anim;
	animFR = fps;
	this->numFrames = numFrames;
}

const uint8_t* Object::animate(){
	if(animation != NULL){
		static uint8_t counter = FRAME_RATE/animFR; // number of times animate() should be called before the sprite changes
		static uint8_t currIndex = 0;
		counter--;
		// change index according to the frame rate
		if(counter == 0){
			currIndex++;
			if(currIndex > numFrames)
				currIndex = 0;
			counter = FRAME_RATE/animFR;
		}
		// return an element in animation array
		return animation[currIndex];
	}
	return defaultAnimation();
}

const uint8_t* Object::defaultAnimation(){
	return NULL;
}

int16_t Object::getType(){
	return OBJECT_TYPE;
}

void Object::draw(){
	if(!isDestoyed()){ // Only visible if not destroyed
		const uint8_t* sprite = animate();
		SSD1306_DrawBMP(XtoPixels(),YtoPixels(),sprite,0,SSD1306_WHITE);
	}
}
