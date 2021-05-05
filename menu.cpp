#include "menu.h"

menu::menu(int language, bool start) {
	this->language = NO_SELECTION;
	this->start = false;
}

void menu::menuInit() {
	Switch s;
	SSD1306_OutClear();
  SSD1306_DrawBMP(0, 63, LanguageMenu, 0, SSD1306_WHITE);
	SSD1306_OutBuffer();
	while(language == NO_SELECTION) {
		if (s.up_Clicked()) {
			language = ENGLISH;
			SSD1306_OutClear();
			SSD1306_DrawBMP(0, 63, TimeParadoxEnglish, 0, SSD1306_WHITE);
			SSD1306_OutBuffer();
		}
		else if (s.down_Clicked()) {
			language = SPANISH;
			SSD1306_OutClear();
			SSD1306_DrawBMP(0, 63, TimeParadoxSpanish, 0, SSD1306_WHITE);
			SSD1306_OutBuffer();
		}
	}
	while(!start) {
		if (s.up_Clicked()) {
			start = true;
			SSD1306_OutClear();
		}
	}
}

int8_t menu::lang() {
	return language;
}
