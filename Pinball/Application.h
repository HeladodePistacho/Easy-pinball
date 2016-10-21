#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Engine/PugiXml\src\pugixml.hpp"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class ModuleSceneIntro;
class ModulePhysics;
class FileSystem;
class ModuleFonts;

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
	FileSystem* file_system;
	ModuleFonts* fonts;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Awake();

	bool Init();
	update_status Update();
	bool CleanUp();


	void AddModule(Module* mod);

	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

public:

	uint				frames;
	float				dt;

	pugi::xml_document general_config;
	pugi::xml_node module_config;


private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;


	p2List<Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;


};

#endif