#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"

#include<string.h>


//Constructor
ModuleFonts::ModuleFonts() :Module()
{}

//Destructor
ModuleFonts::~ModuleFonts()
{}

//Load texture
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows) {
	int id = -1;
	//Bad data enterd
	if (texture_path == nullptr || characters == nullptr || rows == 0) {
		LOG("Could not load fonts");
		return id;
	}
	SDL_Texture* texture = App->textures->Load(texture_path);

	//Charaters over the capacity
	if (texture == nullptr || strlen(characters) >= MAX_FONT_CHARS) {

		LOG("Can't load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; id++) {
		if (fonts[id].graphic == nullptr)break;
	}
	//Characters array if full
	if (id == MAX_FONTS) {
		LOG("Cannot load font %s. Array full (max %d)", texture_path, MAX_FONTS);
		return id;
	}

	//Here starts the bitmap font creation
	Font* f = &fonts[id];
	f->graphic = texture;
	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	f->rows = rows;
	f->lenght = strlen(characters);
	f->row_chars = fonts[id].row_chars / rows;

	uint w, h;
	App->textures->GetSize(texture, w, h);
	f->row_chars = fonts[id].lenght / rows;
	f->char_w = w / fonts[id].row_chars;
	f->char_h = h / rows;

	LOG("BMP font loaded from %s", texture_path);

	return id;
}


void ModuleFonts::UnLoad(int font_id) {
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

// Render text using a bmp font
void ModuleFonts::Blit(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	int digits = 0;
	for (uint i = 0; i < len; ++i)
	{
		for (uint j = 0; j < font->lenght; ++j)
		{
			if (font->table[j] == text[i]) {
				digits++;
			}
		}
	}
	x -= rect.w*digits;
	for (uint i = 0; i < len; ++i)
	{
		// we could use strchr instead ?
		for (uint j = 0; j < font->lenght; ++j)
		{
			if (font->table[j] == text[i])
			{
				rect.x = rect.w * (j % font->row_chars);
				rect.y = rect.h * (j / font->row_chars);
				App->renderer->Blit(font->graphic, x, y, &rect, 0.0f);
				x += rect.w;
				break;
			}
		}
	}
}