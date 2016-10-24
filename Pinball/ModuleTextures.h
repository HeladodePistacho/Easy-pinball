#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "Engine/SDL\include\SDL.h"
#include "Engine/SDL/include/SDL_pixels.h"

#define MAX_FONTS 100
#define MAX_FONT_CHARS 256

struct Font {
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	//Lines, row lenght, font weight, font height, chars in a row
	uint rows, lenght, char_w, char_h, row_chars;
};

enum BLIT_POINT {

	CENTER,
	LEFT,
	RIGHT

};

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Awake(pugi::xml_node&);
	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;



	int LoadFont(const char* texture_path, const char* characters, uint rows = 1);
	//Unload
	void UnLoadFont(int font_id);
	//Create texture from data
	void BlitFont(int x, int y, int bmp_font_id, const char* text, BLIT_POINT blit_point = RIGHT)const;


public:
	p2List<SDL_Texture*> textures;

private:
	Font fonts[MAX_FONTS];
};