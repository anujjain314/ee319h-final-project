// put prototypes for public functions, explain what it does
// put your names here, date
#ifndef __DAC_H__ // do not include more than once
#define __DAC_H__
#include <stdint.h>

// Initialize 6-bit DAC, called once 
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/15
void DAC_Out(uint8_t data);


#endif
