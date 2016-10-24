#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Awake(pugi::xml_node&);
	bool Start();
	update_status Update();
	bool CleanUp();

public:

	uint score = 0;

	uint lives = 3;

	uint total_score = 0;

};