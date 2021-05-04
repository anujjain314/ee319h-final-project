#ifndef _Object_h
#define _Object_h
#include <stdint.h>
#include "SSD1306.h"
#include "Images.h"
#include "trig.h"
#include "Utility.h"
#include "vector.h"

//Max values for position and velocity, based on SSD1306 display
#define MAX_X 12800
#define MAX_Y 6400

#define FRAME_RATE 20
#define NULL 0

#define OBJECT_TYPE 0

class Object {
	protected:
		//fixed point, divide by 100 to get number of pixels
		int16_t x;	//x-position - ranges from 0 to 12,800
		int16_t y;  //y-position - ranges from 0 to 6400
		int16_t vx; //x-velocity
		int16_t vy; //y-velocity
		int16_t maxV; //max velocity
		int16_t ax; //x-acceleration
		int16_t ay; //y-acceleration
	  int16_t size; //size in 0.01 pixels, used for collision calculations
		//direction - Ranges from 0 to 255, where 0 is right, 64 is Down, 128 is left, 192 is up
		uint16_t dir;
		bool destroyed;
		const uint8_t **animation;
		uint8_t animFR; // frame rate of animations, max is FRAME_RATE
		uint8_t numFrames; // num Frames in animation
		uint8_t XtoPixels(); // returns the pixel value associated with the x_pos
		uint8_t YtoPixels(); // returns the pixel value associated with the y_pos
		const uint8_t* animate();      // returns the next sprite to be drawn 
		virtual const uint8_t* defaultAnimation(); // returns the default sprite, used when there is no animation
	public:
		Object(int16_t x, int16_t y, int16_t vx, int16_t vy);
		void move(); 	// changes the x and y positions according to the velocity, changes velocity accoring to the acceleration
		void turn(int16_t speed); //changes the direction by an amount proportional to speed
		void setAcceleration(int16_t a); // sets the acceleration in the current direction (units of a is pixels/s^2)
		// sets the current animation to anim, if anim is null, sets to default animations of object; fps is the frame rate of the animation (max of FRAME_RATE)
		// numFrames is the length of the animation array
		int16_t getX();    // returns x position
		int16_t getY();    // returns y position
	  int16_t getSize();   // returns size
		int16_t getVelocity(); //returns velocity
		int16_t getDirection(); // returns direction
	  virtual int16_t getType();   // returns the Specific type of Object, Ex: SHIP_TYPE, ASTEROID_TYPE, Etc.
		bool isDestoyed(); // returns true if the object is destoyed
	  void destroy();    // destroys the object
	  bool isColliding(Object &other); // returns true if this object is colliding with the other object
		void getCollisions(vector<Object*> &others, vector<Object*> &collisions); //adds all objects in others vector that collide with this object to the collisions vector
		void setAnimation(const uint8_t **anim, uint8_t fps, uint8_t numFrames);
		void draw(); // draws the object on the SSD_1306
};

#endif
