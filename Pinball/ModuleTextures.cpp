#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "FileSystem.h"

#include "Engine/SDL_image/include/SDL_image.h"
#pragma comment( lib, "Engine/SDL_image/libx86/SDL2_image.lib" )


ModuleTextures::ModuleTextures(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.create("textures");
}

// Destructor
ModuleTextures::~ModuleTextures()
{}

bool ModuleTextures::Awake(pugi::xml_node& config)
{

	LOG("ModuleTextures Configuration Loaded!");

	return true;
}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	p2List_item<SDL_Texture*>* item = textures.getFirst();

	while(item != NULL)
	{
		SDL_DestroyTexture(item->data);
		item = item->next;
	}

	textures.clear();
	IMG_Quit();
	return true;
}

int ModuleTextures::LoadFont(const char* texture_path, const char* characters, uint rows) {
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

void ModuleTextures::UnLoadFont(int font_id) {
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void ModuleTextures::BlitFont(int x, int y, int font_id, const char* text, BLIT_POINT blit_point) const
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

	if(blit_point == CENTER)x -= rect.w*digits/2;
	else if (blit_point == RIGHT) x -= rect.w*digits;
	else if (blit_point == LEFT) x += rect.w*digits;

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

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(App->file_system->Load(path), 1);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);

		if(texture == NULL)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			textures.add(texture);
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}

// Free texture from memory
void ModuleTextures::Unload(SDL_Texture* texture)
{
	p2List_item<SDL_Texture*>* item = textures.getFirst();

	while(item != NULL)
	{
		if(item->data == texture)
		{
			SDL_DestroyTexture(item->data);
			textures.del(item);
			break;
		}
		item = item->next;
	}
}

void ModuleTextures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}