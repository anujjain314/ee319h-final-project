#include "menu.h"

menu::menu(int8_t language, bool start, Switch &s) {
	this->language = NO_SELECTION;
	this->start = false;
	this->s = &s;
}

void menu::menuInit() {
	SSD1306_OutClear();
  SSD1306_DrawBMP(0, 63, LanguageMenu, 0, SSD1306_WHITE);
	SSD1306_OutBuffer();
	while(language == NO_SELECTION) {
		if (s->up_Clicked()) {
			language = ENGLISH;
			SSD1306_OutClear();
			SSD1306_DrawBMP(0, 63, TimeParadoxEnglish, 0, SSD1306_WHITE);
			SSD1306_OutBuffer();
		}
		else if (s->down_Clicked()) {
			language = SPANISH;
			SSD1306_OutClear();
			SSD1306_DrawBMP(0, 63, TimeParadoxSpanish, 0, SSD1306_WHITE);
			SSD1306_OutBuffer();
		}
	}
	while(!start) {
		if (s->up_Clicked()) {
			start = true;
			SSD1306_OutClear();
		}
	}
}

void menu::gameOver() {
	if (language == ENGLISH) {
		SSD1306_OutClear();
		SSD1306_OutString("Game Over!");
		SSD1306_SetCursor(0, 1);
		SSD1306_OutString("Score: ");
		SSD1306_OutUDec(finalScore);
	}
	else if (language == SPANISH) {
		SSD1306_OutClear();
		SSD1306_OutString("Juego Terminado!");
		SSD1306_SetCursor(0, 1);
		SSD1306_OutString("Resultado: ");
		SSD1306_OutUDec(finalScore);
	}
}

int8_t menu::lang() {
	return language;
}


