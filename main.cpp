// main.cpp
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 1/16/2021
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/*
 Copyright 2021 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V
// fire button connected to PE0
// special weapon fire button connected to PE1
// 47k  resistor DAC bit 0 on PB0 (least significant bit)
// 24k  resistor DAC bit 1 on PB1
// 12k  resistor DAC bit 2 on PB2
// 6k   resistor DAC bit 3 on PB3
// 3k   resistor DAC bit 4 on PB4
// 1.5k resistor DAC bit 5 on PB5 (most significant bit)

// VCC   3.3V power to OLED
// GND   ground
// SCL   PD0 I2C clock (add 1.5k resistor from SCL to 3.3V)
// SDA   PD1 I2C data

//************WARNING***********
// The LaunchPad has PB7 connected to PD1, PB6 connected to PD0
// Option 1) do not use PB7 and PB6
// Option 2) remove 0-ohm resistors R9 R10 on LaunchPad
//******************************

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "TExaS.h"
#include "SSD1306.h"
#include "Random.h"
#include "Switch.h"
#include "SlidePot.h"
#include "Images.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "Ship.h"
#include "laser.h"
#include "vector.h"
#include "asteroid.h"
#include "Explosion.h"
#include "menu.h"
#include "Sound.h"
#include "Music.h"
#include "Songs.h"

#define ASTEROID_POINTS 1000 // Defines the number of points gained by shooting an asteroid


//********************************************************************************
// debuging profile, pick up to 7 unused bits and send to Logic Analyzer
#define PA54                  (*((volatile uint32_t *)0x400040C0)) // bits 5-4
#define PF321                 (*((volatile uint32_t *)0x40025038)) // bits 3-1
// use for debugging profile
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PA5       (*((volatile uint32_t *)0x40004080))
#define PA4       (*((volatile uint32_t *)0x40004040))
#define MS				80000 //One millisecond at 80 Mhz
// TExaSdisplay logic analyzer shows 7 bits 0,PA5,PA4,PF3,PF2,PF1,0
void LogicAnalyzerTask(void){
  UART0_DR_R = 0x80|PF321|PA54; // sends at 10kHz
}
void ScopeTask(void){  // called 10k/sec
  UART0_DR_R = (ADC1_SSFIFO3_R>>4); // send ADC to TExaSdisplay
}
void Profile_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x21;      // activate port A,F
  while((SYSCTL_PRGPIO_R&0x20) != 0x20){};
  GPIO_PORTF_DIR_R |=  0x0E;   // output on PF3,2,1
  GPIO_PORTF_DEN_R |=  0x0E;   // enable digital I/O on PF3,2,1
  GPIO_PORTA_DIR_R |=  0x30;   // output on PA4 PA5
  GPIO_PORTA_DEN_R |=  0x30;   // enable on PA4 PA5
}
//********************************************************************************

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

//Global Vars
int8_t asteroidTime = 1;
SlidePot pot(185, 66);
uint32_t score = 0;
uint8_t asteroidSpawnRate = 100;
Switch s;
Music music;
Ship* player = new Ship(6000, 3800);
vector<Object*> objs(20);
bool needToDraw = false;
bool needReset = false;
bool needRestart = false;
bool endGame = false;

// returns true if one of the objects is either type1 or type2 and the other object is the other type
bool checkTypes(int16_t type1, int16_t type2, Object* o1, Object* o2){
	return (o1->getType() == type1 && o2->getType() == type2) || (o1->getType() == type2 && o2->getType() == type1);
}

// Handles All Collisons between objects in an object* vector
void handleCollisions(vector<Object*> &objs, uint32_t *score){
	for(uint8_t i = 0; i < objs.len(); i++){
		for(uint8_t j = i + 1; j < objs.len(); j++){
			if(objs[i]->isColliding(*objs[j])){
				if(checkTypes(ASTEROID_TYPE, LASER_TYPE, objs[i], objs[j])){ // ASTEROID-LASER collisions
						if (objs[i]->getType() == ASTEROID_TYPE) {
							static_cast<asteroid*>(objs[i])->breakDown(objs);
							*score += ASTEROID_POINTS;
							}
						else {
							static_cast<asteroid*>(objs[j])->breakDown(objs);
						}
					if(!objs.isFull())
						objs.push_back(new Explosion(objs[i]->getX(), objs[i]->getY()));
					objs[i]->destroy();
					objs[j]->destroy();
					//Add method to break up large asteroids, add explosion
				} else if(checkTypes(ASTEROID_TYPE, SHIP_TYPE, objs[i], objs[j])){ // ASTEROID-SHIP collisions
					if(!objs.isFull())
						objs.push_back(new Explosion(objs[i]->getX(), objs[i]->getY()));
					objs[i]->destroy();
					objs[j]->destroy();
					needReset = true;
					// Game Over or Whatever
				}
			}
		}
	}
}

// Removes all destroyed objects from an object* vector
void removeDestroyed(vector<Object*> &objs){
	for(int8_t i = objs.len() - 1; i >= 0; i--){
		if(objs[i]->isDestoyed()){
			delete objs[i];
			objs.remove(i);
		}
	}
}

void spawnRandomAsteroids(vector<Object*> &objs){
		asteroidTime -= 1;
		if (asteroidTime <= 0 && !objs.isFull()) {
			asteroid::generateRandomAsteroid(objs);
			asteroidTime = asteroidSpawnRate;
		}
}

void outputScore(uint32_t score, uint8_t language){
			char scoreStr[10];
			char output[18];
			Utility::toString(scoreStr, score);
			if (language == ENGLISH) {
				Utility::addStrings(output, (char *)"Score : ", scoreStr);
				SSD1306_DrawString(0,0, output, SSD1306_WHITE);
			}
			else if (language == SPANISH) {
				Utility::addStrings(output, (char *)"Resultado : ", scoreStr);
				SSD1306_DrawString(0,0, output, SSD1306_WHITE);
			}
}

void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period - 1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x0000007;
}

void SysTick_Handler(void){ // every 50 ms
		// Handle Button Presses
		if(s.left_Pressed()){
			player->turn(-20);
		}
		if(s.right_Pressed()){
			player->turn(20);
		}

		// Thrust with slidepot
		int data = ADC_In();
    pot.Save(data);
		player->setAcceleration(pot.ADCsample()/2);
// 		Alternative - Thrust with up button
//		if(s.up_Pressed()){
//			player->setAcceleration(20000);
//		} else{
//			player->setAcceleration(0);
//		}
		if (s.up_Clicked()) {
			endGame = true;
		}
		// Fire laser if down is clicked
		if(s.down_Clicked()){
			if(!objs.isFull()){
				objs.push_back(new Laser());
				player->fire(*static_cast<Laser*>(objs.back()));
			}
		}
		
		// Handle All Objects
		spawnRandomAsteroids(objs);			// Randomly generates asterouids
		handleCollisions(objs, &score); // Handles all Collisions, updates score
		removeDestroyed(objs); 					//  Removes destroyed objects from object vector
		// Move all objects
		for(uint8_t i = 0; i < objs.len(); i++){
			objs[i]->move();
		}

		if(!player->isDestoyed()){	// player gets points for survining
			score = score + 9;
		}
		needToDraw = true;    			// Draw objects in main
}


int main(void){
	PLL_Init();
	Sound_Init();
	ADC_Init(SAC_32);  // turn on ADC, set channel to 5
	SSD1306_Init(SSD1306_SWITCHCAPVCC);
	objs.push_back(player);
	menu m(NO_SELECTION, false, s);
	m.menuInit();
	endGame = false;
	SysTick_Init(50*MS);
	while(true){
		
		if(needToDraw){
			needToDraw = false;
			SSD1306_ClearBuffer();
			//display score
			outputScore(score, m.lang());
			//draw all objects
			for(uint8_t i = 0; i < objs.len(); i++){
				objs[i]->draw();
			}
			SSD1306_OutBuffer();
		}
		
		if(needReset){
			DisableInterrupts();
			needReset = false;
			if(score > 1000)
				score -= 1000;
			for(int8_t i = objs.len() - 1; i >= 0; i--){
				delete objs[i];
				objs.remove(i);
			}
			player = new Ship(6000, 3800);
			objs.push_back(player);
			asteroidSpawnRate /= 2;
			asteroidTime = asteroidSpawnRate;
			EnableInterrupts();
		}
		
		if(needRestart){
			DisableInterrupts();
			needRestart = false;
			score = 0;
			for(int8_t i = objs.len() - 1; i >= 0; i--){
				delete objs[i];
				objs.remove(i);
			}
			player = new Ship(6000, 3800);
			objs.push_back(player);
			asteroidSpawnRate = 100;
			asteroidTime = asteroidSpawnRate;
			EnableInterrupts();
		} 	
		
		if (endGame) {
			DisableInterrupts();
			endGame = false;
			needRestart = true;
			m.gameOver(score);
			EnableInterrupts();
		}
	}
}




// You can't use this timer, it is here for starter code only
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240/2;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
