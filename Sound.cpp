// Sound.cpp

// Sound assets based off the original Space Invaders 
// These arrays create classic sounds!
// Jonathan Valvano
// 1/16/2021
#include <stdint.h>
#include "Sound.h"

void Sound_Off(void);
void playNote(void);
	
const uint8_t *currSound;
uint32_t len;
	
static uint8_t index = 0;		// current location within wave array
static uint8_t volume = 63;	// max value of 63, the volume of the sound to be played
const unsigned short wave[64] = {
  32,35,38,41,44,47,49,52,54,56,58,
  59,61,62,62,63,63,63,62,62,61,59,
  58,56,54,52,49,47,44,41,38,35,
  32,29,26,23,20,17,15,12,10,8,
  6,5,3,2,2,1,1,1,2,2,3,
  5,6,8,10,12,15,17,20,23,26,29
};
	
void Sound_Off(void);

// initialize a 11kHz timer, flags, pointers, counters,and the DAC
void Sound_Init(void){
	DAC_Init();
	Timer1_Init();
	Timer2_Init();
	Sound_Off();
	index = 0;
	volume = 63;
};

// 11 kHz timer ISR
// timer ISR sends one point to DAC each interrupt
// Because TExas sends data to TExasDisplay at 10kHz, you will not be able to toggle a profile pin
// However, you can activate TExaS in analog scope mode and connect PD2 to the DAC output to visualize the sound wave
// Excecuted by the timer isr at 11 kHz, outputs one point to the dac until it reaches then end of the sound
void Sound_Play(){
	if(len == 0){
		Timer1_Stop();
		return;
	}
	uint8_t output = *currSound;
	output = (output*63)/255; // Scaled for 6-bit DAC
	DAC_Out(*currSound);
	currSound++;
	len--;
}

// return true if a sound is still playing
bool Sound_Playing(){
	return len > 0;
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the timer interrupt.
// It starts the sound, and the timer ISR does the output
// feel free to change the parameters
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
	currSound = pt;
	len = count;
  Timer1_Init(*Sound_Play, 7273);
};


// **************Sound_Start*********************
// Start sound output, and set Systick interrupt period 
// Sound continues until Sound_Start called again, or Sound_Off is called
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period, determines pitch of sound
void Sound_Start(uint32_t period){
	if(period > 0){
		Timer2_Init(*playNote, period);
	} else {
		Sound_Off();										// turn sound off if period is zero
	}
}
// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
	index = 0;											// reset waveform index
	DAC_Out(0);											// output a default value of 0 to the DAC
  Timer2_Stop();
}


// **************Sound_SetVolume*********************
// Sets Volume to a new Value
// Inputs: new value for volume
// Output: none
void Sound_SetVolume(uint8_t vol){
	volume = vol;
}

// Executed every 12.5ns*(period) by Timer2
// outputs values from the waveform to the DAC
void playNote(void){
		uint8_t output = wave[index];		//get output from wave array
		output = (output*volume)/63;		//scale output depending on volume
		DAC_Out(output);								//output to DAC
		index = 0x3F&(index+1);					//go to next index (wraps around when reaches final index)
}

