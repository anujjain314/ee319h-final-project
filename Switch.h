// Switch.h
// This software to input from switches or buttons
// Runs on TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/17/21
// Lab number: 10
// Hardware connections
// Switches on PE0-PE3, DOWN is on PE0, LEFT is on PE1, RIGHT is on PE2, UP is on PE3
#ifndef _Switch_h
#define _Switch_h
#include <stdint.h>

#define PE0 (*((volatile uint32_t *)0x40024004))
#define PE1 (*((volatile uint32_t *)0x40024008))
#define PE2 (*((volatile uint32_t *)0x40024010))
#define PE3 (*((volatile uint32_t *)0x40024020))

class Switch {
	bool upPrevious;   //true if up was pressed when last checked
	bool downPrevious; //true if down was pressed when last checked
 	bool leftPrevious; //true if left was pressed when last checked
	bool rightPrevious;//true if right was pressed when last checked
	public:
		Switch(); 						// initialized switches
		bool up_Pressed(); 		// returns true if up button is pressed
		bool down_Pressed();  // returns true if down button is pressed
		bool left_Pressed();  // returns true if left button is pressed
		bool right_Pressed(); // returns true if right button is pressed
		bool up_Clicked(); 		// returns true only once every time up is pressed
		bool down_Clicked();  // returns true only once every time down is pressed
		bool left_Clicked();  // returns true only once every time left is pressed
		bool right_Clicked(); // returns true only once every time right is pressed
};

// Header files contain the prototypes for public functions 
// this file explains what the module does


#endif

