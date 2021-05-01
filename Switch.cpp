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

bool Switch::down_Pressed(){
	return PE0;
}

bool Switch::left_Pressed(){
	return PE1 >> 1;
}

bool Switch::right_Pressed(){
	return PE2 >> 2;
}

bool Switch::up_Pressed(){
	return PE3 >> 3;
}


