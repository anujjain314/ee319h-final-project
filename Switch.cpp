// Switch.cpp
// This software to input from switches or buttons
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/14/21
// Lab number: 10
// Hardware connections
// Switches on PE0-PE3, DOWN is on PE0, LEFT is on PE1, RIGHT is on PE2, UP is on PE3

#include <stdint.h>
#include "Switch.h"
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

Switch::Switch(){
	SYSCTL_RCGCGPIO_R |= 0x10;	// turn on port E clock
	__nop();										// wait for clock to stabilize
	GPIO_PORTE_DIR_R |= 0x00;		// make PE0-PE3 inputs
	GPIO_PORTE_DEN_R |= 0x0F;		// enable PE0-PE3
	GPIO_PORTE_PDR_R |= 0x0F;		// enable Pull-down resistors on PE0-PE3
}

// get status of buttons, don't update previous values
bool up(){
	return PE3 >> 3;
}

bool down(){
	return PE0;
}

bool left(){
	return PE1 >> 1;
}

bool right(){
	return PE2 >> 2;
}

// get status of buttons, update previous values
bool Switch::down_Pressed(){
	downPrevious = down();
	return downPrevious;
}

bool Switch::left_Pressed(){
	leftPrevious = left();
	return leftPrevious;
}

bool Switch::right_Pressed(){
	rightPrevious = right();
	return rightPrevious;
}

bool Switch::up_Pressed(){
	upPrevious = up();
	return upPrevious;
}

// return true if button was pressed after not being pressed, update previous values
bool switchClicked(bool current, bool *previous){
	bool clicked = current && !(*previous);
	*previous = current;
	return clicked;
}

bool Switch::up_Clicked(){
	return switchClicked(up(), &upPrevious);
}

bool Switch::down_Clicked(){
	return switchClicked(down(), &downPrevious);
}

bool Switch::left_Clicked(){
	return switchClicked(left(), &leftPrevious);
}

bool Switch::right_Clicked(){
	return switchClicked(right(), &rightPrevious);
}


