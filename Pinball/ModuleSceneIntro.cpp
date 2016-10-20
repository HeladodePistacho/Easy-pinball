#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
	name.create("scene");
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Awake(pugi::xml_node& config)
{
	LOG("ModuleScene Configuration Loaded!");
	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("Textures/ball.png"); 
	box = App->textures->Load("Textures/crate.png");
	rick = App->textures->Load("Textures/rick_head.png");
	bonus_fx = App->audio->LoadFx("Audio/bonus.wav");

	//Pinball background
	background = App->textures->Load("Textures/background.png");
	//Pinball flaps
	left_flap = App->textures->Load("Textures/left_flap.png");
	right_flap = App->textures->Load("Textures/right_flap.png");


	//Pinball map walls
	int map_walls[204] = {
		428, 910,
		498, 908,
		487, 862,
		526, 844,
		560, 824,
		592, 799,
		627, 762,
		658, 713,
		673, 683,
		681, 653,
		683, 614,
		680, 585,
		666, 552,
		643, 524,
		660, 493,
		689, 447,
		702, 422,
		707, 398,
		707, 382,
		701, 367,
		690, 350,
		675, 333,
		660, 321,
		644, 306,
		660, 273,
		677, 237,
		692, 212,
		704, 191,
		706, 166,
		701, 140,
		684, 110,
		654, 82,
		613, 57,
		564, 37,
		534, 29,
		517, 29,
		502, 34,
		493, 45,
		490, 60,
		491, 85,
		496, 135,
		492, 157,
		465, 151,
		443, 174,
		435, 162,
		432, 144,
		432, 119,
		429, 92,
		422, 68,
		408, 44,
		388, 32,
		362, 23,
		329, 21,
		301, 24,
		273, 33,
		250, 46,
		230, 63,
		212, 82,
		198, 105,
		184, 131,
		174, 157,
		171, 184,
		173, 205,
		184, 224,
		196, 238,
		211, 248,
		228, 260,
		251, 270,
		266, 277,
		239, 287,
		222, 301,
		208, 320,
		200, 342,
		195, 368,
		198, 391,
		204, 412,
		215, 432,
		233, 456,
		247, 480,
		263, 501,
		272, 527,
		270, 540,
		259, 552,
		246, 572,
		238, 594,
		234, 615,
		233, 632,
		235, 654,
		243, 685,
		257, 716,
		271, 738,
		284, 759,
		306, 782,
		324, 797,
		347, 816,
		367, 830,
		384, 840,
		404, 850,
		418, 859,
		434, 868,
		428, 892,
		423, 910
	};
	App->physics->CreateChain(0, 0, map_walls, 204);

	//Left arm
	int left_arm[54] = {
		266, 610,
		266, 633,
		266, 675,
		267, 694,
		270, 713,
		277, 728,
		288, 741,
		302, 749,
		318, 758,
		334, 769,
		351, 779,
		362, 785,
		372, 778,
		373, 766,
		357, 750,
		331, 734,
		311, 718,
		298, 704,
		292, 688,
		290, 670,
		291, 656,
		292, 640,
		293, 627,
		292, 611,
		287, 601,
		278, 599,
		270, 600
	};
	App->physics->CreateChain(0, 0, left_arm, 54);

	//Right arm
	int right_arm[62] = {
		632, 594,
		628, 604,
		628, 614,
		628, 631,
		629, 653,
		629, 671,
		629, 684,
		625, 700,
		617, 712,
		602, 724,
		580, 740,
		558, 753,
		544, 767,
		546, 776,
		550, 783,
		559, 786,
		571, 780,
		590, 765,
		603, 756,
		626, 741,
		639, 729,
		648, 713,
		650, 698,
		650, 681,
		651, 664,
		653, 646,
		654, 628,
		656, 616,
		654, 602,
		649, 594,
		639, 592
	};
	App->physics->CreateChain(0, 0, right_arm, 62);

	//Launcher
	int Launcher[18] = {
		771, 491,
		772, 736,
		800, 736,
		799, 469,
		812, 468,
		811, 746,
		759, 746,
		751, 468,
		772, 470
	};
	App->physics->CreateChain(0, 0, Launcher, 18);

	/*

	*/
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Blit the clean background
	App->renderer->Blit(background, 0, 0);

	//Avtive ball launch
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		p2List_item<PhysBody*>* cir = circles.getFirst();

		while (cir != NULL)
		{
			if (cir->data->Contains(778, 720) || cir->data->Contains(780, 720) || cir->data->Contains(785, 720) || cir->data->Contains(790, 720))
			{
				cir->data->body->ApplyForce({ 0.0f, -100.0f }, cir->data->body->GetPosition(), false);
			}
			cir = cir->next;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		App->physics->PushUpFlaps();

	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE) {

		App->physics->PushDownFlaps();
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	int pos_x, pos_y; 
	App->physics->flap_down_left->GetPosition(pos_x, pos_y);
	App->renderer->Blit(left_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_down_left->GetRotation());

	App->physics->flap_down_right->GetPosition(pos_x, pos_y);
	App->renderer->Blit(right_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_down_right->GetRotation());

	App->physics->flap_up_left->GetPosition(pos_x, pos_y);
	App->renderer->Blit(left_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_up_left->GetRotation());

	App->physics->flap_up_right->GetPosition(pos_x, pos_y);
	App->renderer->Blit(right_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_up_right->GetRotation());
	
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
