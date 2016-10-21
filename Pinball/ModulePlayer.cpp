#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name.create("player");
}

ModulePlayer::~ModulePlayer()
{}

bool ModulePlayer::Awake(pugi::xml_node& config) {


	LOG("ModulePlayer Configuration Loaded!");
	return true;
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	//score_font
    //score_font = App->fonts->Load("Game/Pinball/Textures/score_font.png", "0123456789", 1);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	//Unload all the fonts
	App->fonts->UnLoad(score_font);
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	
	//Draw player score
	sprintf_s(score_text, 10, "%i", score);
	App->fonts->Blit(73, 10, score_font, score_text);

	return UPDATE_CONTINUE;
}



