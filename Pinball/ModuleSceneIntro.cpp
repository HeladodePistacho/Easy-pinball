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

	wheel = App->textures->Load("Textures/wheel.png");
	wheel_flames_1 = App->textures->Load("Textures/wheels_flames_1.png");
	wheel_flames_2 = App->textures->Load("Textures/wheels_flames_2.png");
	wheel_flames_3 = App->textures->Load("Textures/wheels_flames_3.png");
	
	launcher = App->textures->Load("Textures/launcher.png");
	scape_1 = App->textures->Load("Textures/scape_1.png");
	scape_2 = App->textures->Load("Textures/scape_2.png");

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
	App->physics->CreateChain(0, 0, map_walls, 204, MAP);

	//Left arm
	int left_arm[76] = {
		259, 600,
		264, 621,
		267, 636,
		269, 651,
		269, 661,
		270, 675,
		272, 685,
		272, 700,
		271, 713,
		276, 724,
		285, 733,
		296, 742,
		313, 751,
		321, 759,
		333, 767,
		346, 778,
		364, 787,
		374, 786,
		380, 778,
		381, 769,
		372, 761,
		359, 750,
		349, 743,
		333, 734,
		321, 727,
		306, 722,
		294, 715,
		289, 706,
		288, 696,
		287, 685,
		287, 668,
		287, 651,
		287, 632,
		288, 619,
		288, 606,
		286, 595,
		272, 593,
		266, 594
	};
	App->physics->CreateChain(0, 0, left_arm, 76, NONE);

	//Right arm
	int right_arm[74] = {
		634, 598,
		633, 619,
		633, 635,
		633, 648,
		634, 663,
		633, 677,
		632, 694,
		629, 702,
		626, 709,
		619, 717,
		610, 724,
		598, 731,
		587, 735,
		575, 744,
		561, 752,
		551, 762,
		541, 771,
		542, 778,
		545, 783,
		555, 787,
		568, 781,
		586, 768,
		600, 758,
		614, 743,
		621, 741,
		633, 733,
		644, 724,
		650, 707,
		650, 685,
		649, 671,
		649, 654,
		651, 639,
		654, 623,
		657, 613,
		659, 601,
		653, 593,
		644, 593
	};
	App->physics->CreateChain(0, 0, right_arm, 74, NONE);

	//Launcher
	int launcher[98] = {
		666, 483,
		676, 493,
		692, 513,
		707, 536,
		715, 551,
		722, 569,
		727, 585,
		731, 603,
		735, 629,
		736, 652,
		737, 673,
		737, 692,
		737, 713,
		734, 729,
		733, 746,
		731, 757,
		762, 761,
		766, 727,
		769, 700,
		769, 678,
		769, 663,
		768, 636,
		765, 610,
		762, 590,
		756, 569,
		750, 546,
		740, 526,
		729, 504,
		715, 486,
		698, 467,
		686, 456,
		691, 449,
		721, 481,
		745, 517,
		759, 554,
		769, 589,
		774, 631,
		777, 674,
		773, 716,
		771, 750,
		768, 768,
		721, 760,
		728, 705,
		730, 653,
		724, 607,
		715, 569,
		698, 536,
		673, 501,
		661, 489
	};
	App->physics->CreateChain(0, 0, launcher, 98, LAUNCHER);

	//Right lung
	int right_lung[34] = {
		559, 720,
		577, 711,
		599, 697,
		609, 684,
		610, 668,
		610, 654,
		610, 634,
		610, 617,
		606, 604,
		600, 598,
		592, 601,
		586, 616,
		580, 632,
		569, 653,
		564, 674,
		560, 696,
		558, 712
	};
	App->physics->CreateChain(0, 0, right_lung, 34, NONE, 2);

	//Left lung
	int left_lung[32] = {
		364, 713,
		361, 723,
		348, 718,
		315, 697,
		311, 683,
		313, 656,
		313, 627,
		314, 611,
		319, 607,
		326, 607,
		334, 618,
		339, 631,
		349, 647,
		353, 665,
		359, 685,
		361, 699
	};
	App->physics->CreateChain(0, 0, left_lung, 32, NONE, 2);

	//Ramp a
	int ramp_a[322] = {
		239, 385,
		204, 336,
		173, 290,
		160, 259,
		158, 228,
		163, 205,
		174, 184,
		190, 165,
		209, 151,
		235, 137,
		262, 123,
		297, 111,
		323, 103,
		351, 97,
		375, 94,
		402, 92,
		430, 92,
		454, 93,
		483, 97,
		508, 102,
		537, 110,
		566, 119,
		588, 127,
		610, 135,
		632, 143,
		656, 155,
		675, 166,
		692, 177,
		706, 192,
		718, 209,
		727, 228,
		735, 255,
		740, 285,
		740, 319,
		739, 352,
		735, 383,
		729, 422,
		720, 461,
		709, 498,
		696, 538,
		685, 553,
		673, 560,
		661, 565,
		647, 571,
		639, 580,
		634, 591,
		633, 605,
		630, 628,
		601, 623,
		608, 590,
		614, 566,
		624, 552,
		636, 546,
		648, 541,
		660, 536,
		671, 524,
		677, 507,
		682, 491,
		688, 469,
		697, 437,
		704, 410,
		709, 378,
		711, 348,
		712, 320,
		712, 297,
		708, 273,
		701, 250,
		694, 229,
		680, 210,
		667, 196,
		648, 184,
		624, 173,
		597, 162,
		569, 152,
		544, 144,
		507, 138,
		473, 133,
		442, 132,
		417, 130,
		389, 133,
		358, 137,
		328, 143,
		299, 152,
		274, 161,
		250, 173,
		228, 186,
		211, 201,
		203, 213,
		199, 227,
		198, 239,
		198, 253,
		205, 267,
		213, 280,
		222, 297,
		233, 314,
		244, 332,
		256, 348,
		267, 364,
		272, 359,
		240, 320,
		211, 269,
		202, 242,
		207, 218,
		222, 199,
		261, 171,
		312, 153,
		370, 140,
		426, 134,
		515, 146,
		581, 162,
		632, 183,
		668, 206,
		693, 241,
		703, 285,
		705, 330,
		704, 366,
		697, 418,
		687, 455,
		674, 496,
		661, 523,
		648, 528,
		637, 535,
		613, 555,
		604, 579,
		599, 606,
		594, 626,
		639, 636,
		641, 593,
		653, 577,
		670, 569,
		686, 560,
		703, 540,
		713, 504,
		733, 432,
		744, 364,
		745, 323,
		745, 291,
		740, 257,
		728, 219,
		710, 186,
		687, 167,
		649, 142,
		619, 129,
		582, 116,
		533, 101,
		488, 91,
		443, 85,
		391, 85,
		336, 92,
		303, 101,
		273, 112,
		245, 123,
		219, 139,
		179, 168,
		152, 213,
		153, 243,
		161, 274,
		181, 308,
		210, 352,
		225, 374,
		230, 385
	};
	App->physics->CreateChain(0, 0, ramp_a, 322, NONE);

	//Ramp b
	int ramp_b[256] = {
		669, 402,
		695, 359,
		715, 330,
		734, 291,
		746, 267,
		752, 242,
		754, 223,
		756, 201,
		753, 181,
		747, 160,
		738, 139,
		728, 126,
		713, 110,
		690, 90,
		667, 75,
		644, 62,
		615, 48,
		585, 35,
		555, 25,
		523, 19,
		490, 15,
		455, 16,
		420, 22,
		389, 35,
		352, 46,
		317, 59,
		287, 72,
		253, 87,
		221, 105,
		186, 130,
		166, 155,
		144, 194,
		139, 222,
		138, 265,
		146, 311,
		169, 367,
		192, 408,
		214, 440,
		239, 482,
		255, 516,
		273, 548,
		284, 586,
		290, 610,
		293, 619,
		321, 613,
		313, 579,
		301, 546,
		290, 518,
		276, 488,
		253, 453,
		232, 419,
		212, 388,
		193, 355,
		181, 324,
		170, 289,
		165, 255,
		168, 225,
		174, 200,
		186, 179,
		203, 158,
		228, 135,
		261, 115,
		298, 97,
		350, 75,
		398, 62,
		447, 58,
		491, 57,
		536, 62,
		577, 74,
		605, 86,
		630, 102,
		657, 119,
		677, 135,
		693, 147,
		699, 161,
		707, 176,
		712, 196,
		714, 210,
		713, 227,
		710, 247,
		705, 262,
		698, 279,
		687, 299,
		672, 324,
		661, 346,
		647, 369,
		635, 390,
		629, 386,
		675, 309,
		704, 250,
		706, 200,
		691, 156,
		664, 128,
		616, 98,
		546, 70,
		433, 62,
		341, 83,
		252, 125,
		199, 171,
		171, 243,
		179, 300,
		205, 367,
		248, 434,
		287, 503,
		311, 561,
		327, 616,
		288, 626,
		258, 531,
		216, 454,
		160, 358,
		133, 275,
		136, 204,
		171, 141,
		231, 92,
		303, 60,
		360, 37,
		430, 15,
		500, 11,
		567, 24,
		629, 49,
		701, 95,
		747, 144,
		759, 189,
		759, 241,
		733, 300,
		705, 357,
		687, 385,
		673, 405
	};
	App->physics->CreateChain(0, 0, ramp_b, 256, NONE);

	//Ramp c
	int ramp_c[190] = {
		217, 252,
		200, 241,
		187, 234,
		173, 231,
		161, 238,
		154, 251,
		152, 270,
		152, 289,
		152, 315,
		152, 337,
		151, 362,
		151, 384,
		155, 413,
		161, 434,
		172, 454,
		184, 468,
		201, 480,
		216, 489,
		236, 492,
		253, 495,
		271, 501,
		282, 510,
		292, 525,
		305, 556,
		311, 574,
		316, 596,
		318, 610,
		319, 621,
		291, 623,
		288, 602,
		286, 590,
		282, 573,
		277, 559,
		268, 548,
		259, 536,
		247, 527,
		237, 522,
		225, 519,
		208, 515,
		185, 505,
		168, 493,
		153, 478,
		142, 462,
		133, 441,
		127, 420,
		124, 401,
		121, 376,
		117, 343,
		116, 316,
		116, 290,
		120, 264,
		123, 240,
		131, 219,
		142, 205,
		151, 198,
		164, 196,
		182, 195,
		195, 197,
		207, 203,
		221, 209,
		236, 219,
		240, 214,
		203, 193,
		170, 189,
		133, 206,
		124, 226,
		112, 303,
		118, 386,
		126, 431,
		142, 471,
		169, 501,
		196, 514,
		237, 529,
		266, 555,
		279, 582,
		285, 618,
		286, 628,
		322, 628,
		322, 592,
		311, 553,
		299, 527,
		279, 499,
		243, 486,
		209, 480,
		182, 458,
		162, 420,
		154, 374,
		155, 326,
		154, 277,
		162, 248,
		170, 239,
		178, 238,
		190, 241,
		204, 249,
		214, 258
	};
	App->physics->CreateChain(0, 0, ramp_c, 190, NONE);

	//Mid wheel chain
	int mid_wheel_chain[68] = {
		484, 259,
		491, 277,
		501, 288,
		510, 299,
		519, 307,
		529, 311,
		536, 302,
		543, 293,
		553, 290,
		563, 290,
		573, 297,
		578, 305,
		580, 314,
		580, 321,
		578, 329,
		574, 336,
		585, 324,
		589, 312,
		594, 301,
		601, 291,
		605, 279,
		604, 267,
		597, 258,
		589, 252,
		577, 241,
		569, 239,
		564, 242,
		553, 250,
		543, 254,
		527, 254,
		510, 251,
		497, 246,
		486, 241,
		485, 251
	};
	App->physics->CreateChain(0, 0, mid_wheel_chain, 68, NONE);

	//Right wheel chain
	int right_wheel_chain[54] = {
		609, 221,
		624, 228,
		632, 220,
		642, 206,
		648, 192,
		651, 180,
		658, 167,
		664, 154,
		668, 138,
		666, 123,
		654, 111,
		641, 102,
		624, 95,
		609, 94,
		595, 99,
		585, 114,
		577, 131,
		573, 137,
		570, 149,
		581, 151,
		591, 152,
		601, 155,
		611, 167,
		619, 180,
		620, 194,
		618, 204,
		613, 213
	};
	App->physics->CreateChain(0, 0, right_wheel_chain, 54, NONE);

	// Right rocket
	int right_rocket[10] = {
		653, 673,
		676, 673,
		673, 690,
		656, 690,
		655, 678
	};
	App->physics->CreateChain(0, 0, right_rocket, 10, NONE, 4);

	// Left rocket
	int left_rocket[10] = {
		249, 690,
		266, 691,
		266, 700,
		263, 711,
		251, 698
	};
	App->physics->CreateChain(0, 0, left_rocket, 10, NONE, 4);

	App->physics->CreateRectangleSensor(660, 465, 20, 20, DOOR_SENSOR);
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

update_status ModuleSceneIntro::PreUpdate()
{
	App->renderer->Blit(background, 0, 0);
	
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
	
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		App->input->GetMouseX();
		App->input->GetMouseY();
	}

	//Active ball launch
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		p2List_item<PhysBody*>* cir = circles.getFirst();

		while (cir != NULL)
		{
			if (cir->data->Contains(746, 747) || cir->data->Contains(780, 720) || cir->data->Contains(785, 720) || cir->data->Contains(790, 720))
			{
				cir->data->body->ApplyForce({ 0.0f, -200.0f }, cir->data->body->GetPosition(), false);
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
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10, BALL));
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

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64, NONE));
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
	
	App->physics->mid_wheel->GetPosition(pos_x, pos_y);
	App->renderer->Blit(wheel, pos_x - 6, pos_y - 7, NULL, 1.0f, App->physics->mid_wheel->GetRotation());

	App->physics->left_wheel->GetPosition(pos_x, pos_y);
	App->renderer->Blit(wheel, pos_x - 6, pos_y - 7, NULL, 1.0f, App->physics->left_wheel->GetRotation());

	App->physics->right_wheel->GetPosition(pos_x, pos_y);
	App->renderer->Blit(wheel, pos_x - 6, pos_y - 7, NULL, 1.0f, App->physics->right_wheel->GetRotation());

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

	//All superior Renders

	//Blit the launcher
	App->renderer->Blit(launcher, 655, 450);
	App->renderer->Blit(scape_1, 158, 91);
	App->renderer->Blit(scape_2, 121, 16);


	//Blit the wheel flames
	App->renderer->Blit(wheel_flames_1, 425, 54);
	App->renderer->Blit(wheel_flames_2, 585, 95);
	App->renderer->Blit(wheel_flames_3, 474, 228);

	return UPDATE_CONTINUE;
}


update_status ModuleSceneIntro::PostUpdate()
{
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	
	if (bodyB->body->GetFixtureList()->GetFilterData().categoryBits == DOOR_SENSOR && bodyA->body->GetFixtureList()->GetFilterData().categoryBits == BALL)
	{
		b2Filter filter = bodyA->body->GetFixtureList()->GetFilterData();
		filter.maskBits = MAP;
		bodyA->body->GetFixtureList()->SetFilterData(filter);
	}


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
