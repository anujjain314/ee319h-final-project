#ifndef _menu_h
#define _menu_h

#include "Switch.h"
#include "Images.h"
#include "SSD1306.h"

#define NO_SELECTION 0
#define ENGLISH 1
#define SPANISH 2

class menu {
	int8_t language;
	bool start;
	
	public:
		menu(int8_t language, bool start);
	  void menuInit();
		void gameOver();
		int8_t lang();
		int32_t finalScore;
};

#endif
