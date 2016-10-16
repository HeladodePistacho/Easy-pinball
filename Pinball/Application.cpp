#include <iostream> 

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "FileSystem.h"

#include "Application.h"

Application::Application()
{
	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	player = new ModulePlayer(this);
	scene_intro = new ModuleSceneIntro(this);
	physics = new ModulePhysics(this);
	file_system = new FileSystem(this);

	// Main Modules
	
	AddModule(file_system);
	AddModule(window);
	AddModule(physics);
	AddModule(renderer);
	AddModule(textures);
	AddModule(input);
	AddModule(audio);
	

	// Scenes
	AddModule(scene_intro);
	
	// Player
	AddModule(player);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
	list_modules.clear();
}

pugi::xml_node Application::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = file_system->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL) {
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	}
	else 
		ret = config_file.child("config");

	return ret;
}

bool Application::Awake() {

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
	}

	if (ret == true)
	{
		p2List_item<Module*>* item;
		item = list_modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
  			ret = item->data->Update();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

int Application::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* Application::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

const char* Application::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* Application::GetOrganization() const
{
	return organization.GetString();
}