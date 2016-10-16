#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "PugiXml\src\pugixml.hpp"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class ModuleSceneIntro;
class ModulePhysics;
class ModuleFileSystem;

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
	ModuleFileSystem* file_system;

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

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	uint				frames;
	float				dt;
	// TODO 2: Create two new variables from pugui namespace:
	// a xml_document to store the while config file and
	// a xml_node to read specific branches of the xml

	pugi::xml_document general_config;
	pugi::xml_node module_config;


private:

	p2List<Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;


};

extern Application* App;

#endif