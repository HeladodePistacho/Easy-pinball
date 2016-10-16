#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

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

};