#ifndef _FONTS_
#define _FONTS_

#include"Module.h"
#include "Engine/SDL/include/SDL_pixels.h"

#define MAX_FONTS 100
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font {
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	//Lines, row lenght, font weight, font height, chars in a row
	uint rows, lenght, char_w, char_h, row_chars;
};

class ModuleFonts : public Module {
public:

	ModuleFonts();
	~ModuleFonts();

	//Functions
	//Load
	int Load(const char* texture_path, const char* characters, uint rows = 1);
	//Unload
	void UnLoad(int font_id);
	//Create texture from data
	void Blit(int x, int y, int bmp_font_id, const char* text)const;

private:
	//Array of char textures
	Font fonts[MAX_FONTS];
};
#endif