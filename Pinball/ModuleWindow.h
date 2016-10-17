#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Engine/SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Awake(pugi::xml_node&);
	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:

	p2SString	title;
	uint		width;
	uint		height;
	uint		scale;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreen_window = false;
	bool vsync = true;
};

#endif // __ModuleWindow_H__