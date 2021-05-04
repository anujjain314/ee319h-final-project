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

void removeAndDestroy(vector<Object*> &v, uint8_t i, uint8_t j){ // removes and destroyed objects at indices i and j
	v[j]->destroy();
	v[i]->destroy();
	v.remove(i);
	v.remove(j);
}

void removeAndDestroy(vector<Object*> &v, uint8_t i){ // removes and destroyed object at indices i
	v[i]->destroy();
	v.remove(i);
}

bool collisionBetween(int16_t type1, int16_t type2, Object* o1, Object* o2){
	return (o1->getType() == type1 && o2->getType() == type2) || (o1->getType() == type2 && o2->getType() == type1);
}

// Polymorphism Test
int main(void){ Switch s; Ship* player = new Ship(6000, 3800); vector<Object*> objs(10);
	PLL_Init();
	SSD1306_Init(SSD1306_SWITCHCAPVCC);
	objs.push_back(player);
	objs.push_back(new asteroid(2000,2000, 47, 20, asteroid_small));
	objs.push_back(new asteroid(4000,2000, 157, 10, asteroid_large));
	while(true){
		SSD1306_ClearBuffer();
		Delay100ms(1);
		
		// Handle Button Presses
		if(s.left_Pressed()){
			player->turn(-20);
		} 
		else if(s.right_Pressed()){
			player->turn(20);
		}
		if(s.up_Pressed()){
			player->setAcceleration(200);
		} else{
			player->setAcceleration(0);
		}
		if(s.down_Clicked()){
			if(objs.push_back(new Laser())){
				player->fire(*static_cast<Laser*>(objs.back()));
			}
		}
		
		// Handle Objects
		for(uint8_t i = 0; i < objs.len(); i++){
			// Handle All Collisons
			for(int j = 0; j < objs.len(); j++){
				if(i != j && objs[i]->isColliding(*objs[j])){
					if(collisionBetween(ASTEROID_TYPE, LASER_TYPE, objs[i], objs[j])){
						objs.push_back(new Explosion(objs[i]->getX(), objs[i]->getY()));
						removeAndDestroy(objs, i, j);
						//Add method to break up large asteroids, add explosion
					} else if(collisionBetween(ASTEROID_TYPE, SHIP_TYPE, objs[i], objs[j])){
						objs.push_back(new Explosion(objs[i]->getX(), objs[i]->getY()));
						removeAndDestroy(objs, i, j);
						// Game Over or Whatever
					}
				}
			}
			
			// remove destroyed objects from vector
			if(objs[i]->isDestoyed()){
				delete objs[i];
				objs.remove(i);
			}
			// move and draw all objects
			objs[i]->move();
			objs[i]->draw();
		}
		
		// Output to screen
		SSD1306_OutBuffer();
	}
}





// Test Player Class, Laser Class, Asteroid Class, dont try to understand this spaghetti code, random testing
int main5(void){ Switch s; Ship player(6000, 3800); vector<Laser*> v(10); vector<asteroid*> v2(10);
	PLL_Init();
	SSD1306_Init(SSD1306_SWITCHCAPVCC);
	v2.push_back(new asteroid(2000,2000, 47, 20, asteroid_small));
	v2.push_back(new asteroid(4000,2000, 157, 10, asteroid_large));
	while(true){
		SSD1306_ClearBuffer();
		Delay100ms(1);
		if(s.left_Pressed()){
			player.turn(-20);
		} 
		else if(s.right_Pressed()){
			player.turn(20);
		}
		if(s.up_Pressed()){
			player.setAcceleration(200);
		} else{
			player.setAcceleration(0);
		}
		if(s.down_Clicked()){
			if(!v.isFull()){
				v.push_back(new Laser());
				player.fire(*v.back());
			}
		}
		for(uint8_t i = 0; i < v.len(); i++){
			
			for(int j = 0; j < v2.len(); j++){
				if(v[i]->isColliding(*v2[j])){
					v2[j]->destroy();
					v[i]->destroy();
				}
			}
			
			if(v[i]->isDestoyed()){
				v.remove(i);
			}
			v[i]->move();
			v[i]->draw();
		}
		
		for(uint8_t i = 0; i < v2.len(); i++){
			if(v2[i]->isDestoyed()){
				v2.remove(i);
			}
			
			if(v2[i]->isColliding(player)){
				player.destroy();
			}
			
			v2[i]->move();
			v2[i]->draw();
		}
		
			player.move();
			player.draw();
		  SSD1306_OutBuffer();
	}
}


// Test Player Class
int main4(void){ Switch s; Ship player(6000, 3800); Laser *l[10];
	uint8_t numLazers = 0;
	PLL_Init();
	SSD1306_Init(SSD1306_SWITCHCAPVCC);
	while(true){
		SSD1306_ClearBuffer();
		Delay100ms(1);
		if(s.left_Pressed()){
			player.turn(-20);
		} 
		else if(s.right_Pressed()){
			player.turn(20);
		}
		if(s.up_Pressed()){
			player.setAcceleration(200);
		} else{
			player.setAcceleration(0);
		}
		if(s.down_Pressed()){
			if(numLazers == 10){
				for(uint8_t i = 0; i < numLazers; i++){
					delete l[i];
				}
				numLazers = 0;
			}
			numLazers++;
			l[numLazers-1] = new Laser();
			player.fire(*l[numLazers-1]);
		}
		for(uint8_t i = 0; i < numLazers; i++){
			l[i]->move();
			l[i]->draw();
		}
			player.move();
			player.draw();
		  SSD1306_OutBuffer();
	}
}

// Test Player Class
int main3(void){ Switch s; Ship player(6000, 3800); Laser l[10];
	uint8_t numLazers = 0;
	PLL_Init();
	SSD1306_Init(SSD1306_SWITCHCAPVCC);
	while(true){
		SSD1306_ClearBuffer();
		Delay100ms(1);
		if(s.left_Pressed()){
			player.turn(-20);
		} 
		else if(s.right_Pressed()){
			player.turn(20);
		}
		if(s.up_Pressed()){
			player.setAcceleration(200);
		} else{
			player.setAcceleration(0);
		}
		if(s.down_Pressed()){
			if(numLazers == 10){
				numLazers = 0;
			}
			numLazers++;
			player.fire(l[numLazers-1]);
		}
		for(uint8_t i = 0; i < numLazers; i++){
			l[i].move();
			l[i].draw();
		}
			player.move();
			player.draw();
		  SSD1306_OutBuffer();
	}
}

// Test Switch Class
int main2(void){uint8_t prev; uint8_t curr; Switch s; uint8_t count;
	while(true){
		curr = s.up_Pressed() || s.down_Pressed() || s.left_Pressed() || s.right_Pressed();
		if(curr != prev)
			count++;
		prev = curr;
	}
}


int main1(void){uint32_t time=0;
  DisableInterrupts();
  // pick one of the following three lines, all three set to 80 MHz
  //PLL_Init();                   // 1) call to have no TExaS debugging
  TExaS_Init(&LogicAnalyzerTask); // 2) call to activate logic analyzer
  //TExaS_Init(&ScopeTask);       // or 3) call to activate analog scope PD2
  SSD1306_Init(SSD1306_SWITCHCAPVCC);
  SSD1306_OutClear();   
  Random_Init(1);
  Profile_Init(); // PB5,PB4,PF3,PF2,PF1 
  SSD1306_ClearBuffer();
  SSD1306_DrawBMP(2, 62, SpaceInvadersMarquee, 0, SSD1306_WHITE);
  SSD1306_OutBuffer();
  EnableInterrupts();
  Delay100ms(20);
  SSD1306_ClearBuffer();
  SSD1306_DrawBMP(47, 63, PlayerShip0, 0, SSD1306_WHITE); // player ship bottom
  SSD1306_DrawBMP(53, 55, Bunker0, 0, SSD1306_WHITE);

  SSD1306_DrawBMP(0, 9, Alien10pointA, 0, SSD1306_WHITE);
  SSD1306_DrawBMP(20,9, Alien10pointB, 0, SSD1306_WHITE);
  SSD1306_DrawBMP(40, 9, Alien20pointA, 0, SSD1306_WHITE);
  SSD1306_DrawBMP(60, 9, Alien20pointB, 0, SSD1306_WHITE);
  SSD1306_DrawBMP(80, 9, Alien30pointA, 0, SSD1306_WHITE);
  SSD1306_DrawBMP(50, 19, AlienBossA, 0, SSD1306_WHITE);
  SSD1306_OutBuffer();
  Delay100ms(30);

  SSD1306_OutClear();  
  SSD1306_SetCursor(1, 1);
  SSD1306_OutString((char *)"GAME OVER");
  SSD1306_SetCursor(1, 2);
  SSD1306_OutString((char *)"Nice try,");
  SSD1306_SetCursor(1, 3);
  SSD1306_OutString((char *)"Earthling!");
  SSD1306_SetCursor(2, 4);
  while(1){
    Delay100ms(10);
    SSD1306_SetCursor(19,0);
    SSD1306_OutUDec2(time);
    time++;
    PF1 ^= 0x02;
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


