#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name.create("window");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

bool ModuleWindow::Awake(pugi::xml_node& config)
{
	LOG("Awake SDL window & surface");
	bool ret = false;

	if(config != NULL)
	{
		//Define window
		fullscreen = config.child("fullscreen").attribute("value").as_bool();
		borderless = config.child("borderless").attribute("value").as_bool();
		resizable = config.child("resizable").attribute("value").as_bool();
		fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool();
		width = config.child("resolution").attribute("width").as_int();
		height = config.child("resolution").attribute("height").as_int();
		scale = config.child("resolution").attribute("scale").as_int();
		LOG("ModuleWindow Configuration Loaded!");
		ret = true;
	}
	else LOG("Configuration XML read error.");

	return ret;
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title.GetString(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}