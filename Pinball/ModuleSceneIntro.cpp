#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include <cmath>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	App->player->stand_lives = 0;

	game_state = NO_GAME;

	
	SDL_Texture* on = App->textures->Load("Textures/new_game_on.png");
	SDL_Texture* off = App->textures->Load("Textures/new_game_off.png");

	SDL_Rect temp;
	temp.x = 943;
	temp.y = 717;
	temp.h = temp.w = 50;

	new_game_button.on_texture = on;
	new_game_button.off_texture = off;
	new_game_button.active_area = temp;
	new_game_button.current_texture = on;

	on = App->textures->Load("Textures/volume_on.png");
	off = App->textures->Load("Textures/volume_off.png");

	temp.x = 832;
	temp.y = 525;
	temp.h = temp.w = 50;

	volume_button.on_texture = on;
	volume_button.off_texture = off;
	volume_button.active_area = temp;
	volume_button.current_texture = on;

	on = App->textures->Load("Textures/launch_on.png");
	off = App->textures->Load("Textures/launch_off.png");

	temp.x = 846;
	temp.y = 716;
	temp.h = temp.w = 50;

	launch_button.on_texture = on;
	launch_button.off_texture = off;
	launch_button.active_area = temp;
	launch_button.current_texture = on;
	



	pause_rect.x = 945;
	pause_rect.y = 536;
	pause_rect.h = pause_rect.w = 50;

	unpause_rect.x = 486;
	unpause_rect.y = 621;
	unpause_rect.h = 31;
	unpause_rect.w = 140;



	


	score_font = App->textures->LoadFont("Textures/test_source.png", ".0123456789", 1);

	debug_font = App->textures->LoadFont("Textures/debug_font.png", "!*#$%&`()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[[]|`", 1);

	speed_font = App->textures->LoadFont("Textures/orange_numbers.png", "1234567890", 1);

	balls_font = speed_font;



	App->audio->PlayMusic("Audio/pinball_theme.ogg");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball = App->textures->Load("Textures/ball.png"); 
	
	flap_up_fx = App->audio->LoadFx("Audio/flap_up_fx.wav");
	flap_down_fx = App->audio->LoadFx("Audio/flap_down_fx.wav");
	launcher_fx = App->audio->LoadFx("Audio/launcher_fx.wav");
	ramp_a_fx = App->audio->LoadFx("Audio/ramp_a_fx.wav");

	flap_up_fx = App->audio->LoadFx("Audio/flap_up_fx.wav");
	flap_down_fx = App->audio->LoadFx("Audio/flap_down_fx.wav");
	launcher_fx = App->audio->LoadFx("Audio/launcher_fx.wav");
	
	ramp_a_fx = App->audio->LoadFx("Audio/ramp_a_fx.wav");
	drift_1_fx = App->audio->LoadFx("Audio/drift_1.wav");
	drift_2_fx = App->audio->LoadFx("Audio/drift_2.wav");
	special_ramp_fx = App->audio->LoadFx("Audio/special_ramp_fx.wav");

	ping_fx = App->audio->LoadFx("Audio/ping_fx.wav");
	ding_ding_fx = App->audio->LoadFx("Audio/ding_ding_fx.wav");
	jackpot_fx = App->audio->LoadFx("Audio/jackpot_fx.wav");
	points_fx = App->audio->LoadFx("Audio/points_fx.wav");

	crash_1_fx = App->audio->LoadFx("Audio/crash_1.wav");
	crash_2_fx = App->audio->LoadFx("Audio/crash_2.wav");

	lung_boom_fx = App->audio->LoadFx("Audio/lung_boom_fx.wav");
	
	wheels_1_fx = App->audio->LoadFx("Audio/wheel_fx.wav");
	wheels_2_fx = App->audio->LoadFx("Audio/wheel_fx_2.wav");

	//Pinball background
	background = App->textures->Load("Textures/background_clean.png");
	
	//Pinball flaps
	left_flap = App->textures->Load("Textures/left_flap.png");
	right_flap = App->textures->Load("Textures/right_flap.png");

	pause = App->textures->Load("Textures/pause_sprite.png");
	instructions = App->textures->Load("Textures/instructions.png");
	end_phrase = App->textures->Load("Textures/end_texture.png");

	wheel = App->textures->Load("Textures/wheel.png");
	wheel_off = App->textures->Load("Textures/wheel_off.png");
	wheel_flames_1 = App->textures->Load("Textures/wheels_flames_1.png");
	wheel_flames_2 = App->textures->Load("Textures/wheels_flames_2.png");
	wheel_flames_3 = App->textures->Load("Textures/wheels_flames_3.png");
	hot_rod_wheels_light = App->textures->Load("Textures/hot_rod_repair_light.png");

	launcher = App->textures->Load("Textures/launcher.png");
	scape_1 = App->textures->Load("Textures/scape_1.png");
	scape_2 = App->textures->Load("Textures/scape_2.png");

	scape_light_1 = App->textures->Load("Textures/scape_light_1.png");
	scape_light_2 = App->textures->Load("Textures/scape_light_2.png");
	scape_light_3 = App->textures->Load("Textures/scape_light_3.png");
	scape_light_4 = App->textures->Load("Textures/scape_light_4.png");
	scape_light_5 = App->textures->Load("Textures/scape_light_5.png");
	scape_light_6 = App->textures->Load("Textures/scape_light_6.png");

	down_red_light = App->textures->Load("Textures/red_light.png");
	down_yellow_light = App->textures->Load("Textures/yellow_light.png");
	down_orange_light = App->textures->Load("Textures/orange_light.png");

	mid_red_light = App->textures->Load("Textures/mid_red_light.png");
	mid_orange_light = App->textures->Load("Textures/mid_orange_light.png");
	mid_yellow_light = App->textures->Load("Textures/mid_yellow_light.png");

	up_light_1 = App->textures->Load("Textures/up_light_1.png");
	up_light_2 = App->textures->Load("Textures/up_light_2.png");
	up_light_3 = App->textures->Load("Textures/up_light_3.png");

	left_lung = App->textures->Load("Textures/left_lung.png");
	right_lung = App->textures->Load("Textures/right_lung.png");

	
	//Pinball map walls
	int map_walls[172] = {
	434, 860,
	430, 938,
	497, 939,
	488, 864,
	531, 852,
	591, 812,
	650, 762,
	675, 722,
	692, 666,
	690, 599,
	680, 558,
	653, 531,
	672, 494,
	689, 460,
	703, 417,
	711, 387,
	705, 370,
	679, 343,
	664, 331,
	642, 308,
	657, 280,
	674, 234,
	689, 204,
	699, 173,
	697, 138,
	680, 99,
	642, 70,
	601, 50,
	550, 40,
	514, 41,
	496, 53,
	492, 74,
	495, 85,
	495, 108,
	497, 136,
	495, 155,
	470, 154,
	443, 170,
	431, 155,
	436, 127,
	431, 101,
	425, 79,
	416, 56,
	402, 36,
	358, 24,
	327, 23,
	295, 30,
	261, 40,
	240, 63,
	237, 88,
	248, 122,
	262, 140,
	285, 159,
	302, 174,
	327, 190,
	343, 209,
	327, 226,
	307, 250,
	282, 238,
	260, 226,
	229, 207,
	200, 195,
	169, 184,
	147, 200,
	146, 220,
	210, 251,
	264, 278,
	233, 291,
	215, 312,
	197, 343,
	195, 384,
	214, 412,
	230, 441,
	249, 470,
	268, 505,
	274, 528,
	268, 557,
	242, 577,
	230, 613,
	232, 638,
	240, 700,
	250, 730,
	270, 761,
	310, 788,
	354, 820,
	393, 841
	};	
	App->physics->CreateChain(0, 0, map_walls, 172, MAP);

	//Map sector
	int map_sector[38] = {
		356, 365,
		351, 392,
		341, 420,
		333, 438,
		329, 448,
		312, 428,
		298, 399,
		284, 375,
		273, 356,
		263, 339,
		254, 327,
		260, 318,
		269, 313,
		281, 311,
		299, 316,
		315, 322,
		325, 334,
		340, 347,
		352, 357
	};
	App->physics->CreateChain(0, 0, map_sector, 38, MAP);


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
	App->physics->CreateChain(0, 0, left_arm, 76, MAP);

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
	App->physics->CreateChain(0, 0, right_arm, 74, MAP);

	//Launcher
	
	int launcher[66] = {
		732, 745,
		762, 748,
		765, 720,
		769, 688,
		769, 655,
		766, 617,
		758, 576,
		748, 543,
		736, 513,
		716, 488,
		701, 472,
		678, 452,
		666, 441,
		676, 434,
		760, 534,
		787, 662,
		772, 762,
		721, 760,
		719, 659,
		714, 595,
		684, 532,
		657, 479,
		671, 488,
		685, 503,
		701, 526,
		708, 544,
		720, 572,
		728, 602,
		734, 630,
		735, 657,
		735, 680,
		736, 707,
		733, 730
	};
	App->physics->CreateChain(0, 0, launcher, 66, LAUNCHER);

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
	App->physics->CreateChain(0, 0, right_lung, 34, MAP, 1);

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
	App->physics->CreateChain(0, 0, left_lung, 32, MAP, 2);

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
	App->physics->CreateChain(0, 0, ramp_a, 322, RAMP_A);

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
	App->physics->CreateChain(0, 0, ramp_b, 256, RAMP_B);

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
	App->physics->CreateChain(0, 0, ramp_c, 190, RAMP_C);

	//Mid wheel chain
	int mid_wheel_chain[64] = {
		529, 312,
		515, 302,
		502, 288,
		492, 277,
		485, 263,
		482, 253,
		487, 244,
		491, 238,
		501, 235,
		517, 232,
		538, 232,
		553, 235,
		567, 237,
		577, 242,
		590, 251,
		594, 258,
		602, 271,
		605, 283,
		600, 293,
		594, 303,
		587, 317,
		581, 334,
		576, 338,
		571, 333,
		576, 319,
		578, 308,
		573, 298,
		564, 292,
		553, 290,
		544, 293,
		537, 302,
		535, 309
	};
	App->physics->CreateChain(0, 0, mid_wheel_chain, 64, MAP);


	//Right wheel chain
	int right_wheel_chain[50] = {
		603, 210,
		610, 220,
		619, 226,
		627, 230,
		634, 227,
		638, 219,
		642, 212,
		649, 195,
		655, 178,
		659, 165,
		666, 144,
		664, 122,
		654, 111,
		642, 106,
		633, 104,
		619, 103,
		606, 108,
		593, 117,
		585, 132,
		584, 144,
		589, 157,
		596, 169,
		600, 183,
		600, 197,
		600, 202
	};
	App->physics->CreateChain(0, 0, right_wheel_chain, 50, MAP);


	// Right rocket
	int right_rocket[10] = {
		653, 673,
		676, 673,
		673, 690,
		656, 690,
		655, 678
	};
	App->physics->CreateChain(0, 0, right_rocket, 10, MAP, 4);

	// Left rocket
	int left_rocket[10] = {
		249, 690,
		266, 691,
		266, 700,
		263, 711,
		251, 698
	};
	App->physics->CreateChain(0, 0, left_rocket, 10, MAP, 4);

	//sensors
	//door sensor
	App->physics->CreateRectangleSensor(570, 400, 20, 20, SENSOR, DOOR);

	//final ramp sensor
	App->physics->CreateRectangleSensor(615, 627, 20, 10, FINAL_RAMP);
	App->physics->CreateRectangleSensor(293, 616, 20, 10, FINAL_RAMP);

	//sensor to enter each ramp
	App->physics->CreateRectangleSensor(218, 230, 10, 10, SENSOR, SENSOR_RAMP_C);
	App->physics->CreateRectangleSensor(648, 389, 10, 10, SENSOR, SENSOR_RAMP_B);
	App->physics->CreateRectangleSensor(241, 357, 10, 10, SENSOR, SENSOR_RAMP_A);

	//sensors that apply a force to the ball

	App->physics->CreateRectangleSensor(241, 357, 10, 10, SENSOR, TURBO_UP);
	App->physics->CreateRectangleSensor(668, 367, 20, 20, SENSOR, TURBO_UP);

	App->physics->CreateRectangleSensor(410, 143, 20, 20, SENSOR, TURBO_UP);
	App->physics->CreateRectangleSensor(410, 143, 20, 20, SENSOR, RAMP_LIGHT_UP);
	App->physics->CreateRectangleSensor(295, 146, 20, 20, SENSOR, TURBO_DOWN);

	
	App->physics->CreateRectangleSensor(300, 283, 10, 10, SENSOR, TURBO_UP_LEFT);

	//3 lights under the top left flap
	//App->physics->CreateRectangleSensor(365, 372, 10, 10, YELLOW_LIGHT);
	//App->physics->CreateRectangleSensor(360, 390, 10, 10, YELLOW_LIGHT);

	int yellow_light_sensor[8] = 
	{
		363, 366, 
		369, 368, 
		355, 393, 
		362, 394 
	};
	App->physics->CreateSensorChain(0, 0, yellow_light_sensor, 8, YELLOW_LIGHT);

	App->physics->CreateRectangleSensor(353, 406, 10, 10, ORANGE_LIGHT);
	App->physics->CreateRectangleSensor(348, 424, 10, 10, ORANGE_LIGHT);

	App->physics->CreateRectangleSensor(340, 439, 10, 10, RED_LIGHT);
	App->physics->CreateRectangleSensor(335, 457, 10, 10, RED_LIGHT);

	//3 light under the car
	App->physics->CreateRectangleSensor(376, 195, 10, 10, YELLOW_LIGHT_UP);
	App->physics->CreateRectangleSensor(367, 208, 10, 10, YELLOW_LIGHT_UP);

	App->physics->CreateRectangleSensor(355, 228, 10, 10, ORANGE_LIGHT_UP);
	App->physics->CreateRectangleSensor(347, 242, 10, 10, ORANGE_LIGHT_UP);

	App->physics->CreateRectangleSensor(333, 256, 10, 10, RED_LIGHT_UP);
	App->physics->CreateRectangleSensor(323, 266, 10, 10, RED_LIGHT_UP);

	//Lights over the wheels
	App->physics->CreateRectangleSensor(505, 100, 20, 20, SENSOR, WHEEL_LIGHT_LEFT);
	App->physics->CreateRectangleSensor(535, 100, 20, 20, SENSOR, WHEEL_LIGHT_MID);
	App->physics->CreateRectangleSensor(565, 100, 20, 20, SENSOR, WHEEL_LIGHT_RIGHT);

	//Jackpot Sensor
	App->physics->CreateRectangleSensor(555, 315, 20, 20, SENSOR, JACKPOT);

	//left and right ramps sensors
	App->physics->CreateRectangleSensor(685, 408, 20, 20, SENSOR, RAMP_LIGHT_LEFT);
	App->physics->CreateRectangleSensor(216, 404, 20, 20, SENSOR, RAMP_LIGHT_RIGHT);
	App->physics->CreateRectangleSensor(600, 320, 20, 20, STOP_SENSOR);
	App->physics->CreateRectangleSensor(328, 293, 10, 40, STOP_SENSOR);

	//The loser sensor
	App->physics->CreateRectangleSensor(460, 881, 70, 10, SENSOR, END);

	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->UnLoadFont(score_font);
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
	if (game_state != PAUSE) {
		int crone_time = GetTickCount();

		if (crone_time >= (initial_time + ratio))
		{
			seconds++;
			if (seconds > 9)
			{
				seconds = 0;
				++decimes;
			}

			if (decimes > 5)
			{
				decimes = 0;
				++minutes;
			}
			initial_time = crone_time;
		}
	}

	p2List_item<PhysBody*>* cir = circles.getFirst();
	bool active = false;
	while (cir != NULL)
	{

		if (cir->data->Contains(752, 735))
		{
			if (launch_button.current_texture != launch_button.on_texture)launch_button.current_texture = launch_button.on_texture;
			active = true;
		}

		cir = cir->next;
	}

	if (active == false)launch_button.current_texture = launch_button.off_texture;


	//DEBUG------------------------------------------------------
	
	//time title
	sprintf_s(time_text, 6, "%s", "TIME");
	App->textures->BlitFont(36, 45, debug_font, time_text);
	
	//time number
	sprintf_s(time_count_text, 14, "%i:%i%i", minutes, decimes, seconds);
	App->textures->BlitFont(100, 45, debug_font, time_count_text);
	
	//rotations

	float rotation = 0;

	sprintf_s(temp_text, 17, "%s", "FLAPS.ROTATION");
	App->textures->BlitFont(80, 60, debug_font, temp_text);

	sprintf_s(temp_text, 10, "%s", "UP.LEFT:");
	App->textures->BlitFont(53, 75, debug_font, temp_text);

	rotation = App->physics->flap_up_left->body->GetAngle() * RADTODEG;
	sprintf_s(debug_text, 100, "%f", rotation);
	App->textures->BlitFont(170, 75, debug_font, debug_text);

	sprintf_s(temp_text, 12, "%s", "DOWN.LEFT:");
	App->textures->BlitFont(61, 90, debug_font, temp_text);

	rotation = App->physics->flap_down_left->body->GetAngle() * RADTODEG;
	sprintf_s(debug_text, 100, "%f", rotation);
	App->textures->BlitFont(170, 90, debug_font, debug_text);

	sprintf_s(temp_text, 11, "%s", "UP.RIGHT:");
	App->textures->BlitFont(56, 105, debug_font, temp_text);

	rotation = App->physics->flap_up_right->body->GetAngle() * RADTODEG;
	sprintf_s(debug_text, 100, "%f", rotation);
	App->textures->BlitFont(170, 105, debug_font, debug_text);

	sprintf_s(temp_text, 13, "%s", "DOWN.RIGHT:");
	App->textures->BlitFont(65, 120, debug_font, temp_text);

	rotation = App->physics->flap_down_right->body->GetAngle() * RADTODEG;
	sprintf_s(debug_text, 100, "%f", rotation);
	App->textures->BlitFont(170, 120, debug_font, debug_text);

	
	if (ball_body != nullptr) {
	
	//score
	sprintf_s(score_text, 10, "%i", App->player->score);
	App->textures->BlitFont(890, 170, score_font, score_text,CENTER);

	if (circles.count() > 0 && ball_body != circles.getLast()->data)ball_body = circles.getLast()->data;

	//mph
		float velocity_x = ball_body->body->GetLinearVelocity().x;
		float velocity_y = ball_body->body->GetLinearVelocity().y;
		int velocity = sqrt((velocity_x*velocity_x) + (velocity_y* velocity_y));
		sprintf_s(speed_text, 100, "%i", velocity);

	}
	else {

		sprintf_s(speed_text, 10, "%i", 0);

	}

	App->textures->BlitFont(965, 260, speed_font, speed_text);

	//balls
	
	sprintf_s(balls_text, 10, "%i", Balls_count);
	App->textures->BlitFont(825, 260, balls_font, balls_text);
	
	//debug tool
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
			if (cir->data->Contains(752, 735))
			{
				cir->data->body->ApplyForce({ 0.0f, -150.0f }, cir->data->body->GetPosition(), true);
				App->audio->PlayFx(launcher_fx);
			}
			
			cir = cir->next;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN && new_game_button.current_texture == new_game_button.on_texture) {

			game_state == START;

	}


	//Check if ball is into jackpot and stop it for 2 seconds
	int current_time = SDL_GetTicks();

	if (ball_body != nullptr) {

		if (ball_into_jackpot && first_time)
		{
			stop_ball = current_time;
			first_time = false;
		}

		if (current_time <= stop_ball + 2000 && ball_into_jackpot)
		{
			ball_body->body->SetTransform({ PIXEL_TO_METERS(556), PIXEL_TO_METERS(313) }, 0);
			ball_body->body->SetLinearVelocity({ -3.0f, 3.0f });
		}
		else
		{
			ball_into_jackpot = false;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

		switch_up_lights();
		App->audio->PlayFx(flap_up_fx);
		App->physics->PushUpLeftFlaps();
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {

		App->physics->PushUpLeftFlaps();

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) {

		App->audio->PlayFx(flap_down_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE) {

		App->physics->PushDownLeftFlaps();
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		switch_up_lights();
		App->audio->PlayFx(flap_up_fx);
		App->physics->PushUpRightFlaps();

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {

		App->physics->PushUpRightFlaps();

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) {

		App->audio->PlayFx(flap_down_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE) {

		App->physics->PushDownRightFlaps();
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && game_state != NO_GAME && game_state != END_GAME) {
		if (ball_body != nullptr) {

			if (ball_body->Contains(752, 735)) {

				ball_body->body->ApplyForce({ 0.0f, -150.0f }, ball_body->body->GetPosition(), true);
				App->audio->PlayFx(launcher_fx);

			}

			if (game_state == START)game_state = IN_GAME;

		}

	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		if (game_state == PAUSE)game_state = last_game_state;

		else {

			last_game_state = game_state;
			game_state = PAUSE;

		}

	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {

		SDL_Point temp;

		temp.x = App->input->GetMouseX();
		temp.y = App->input->GetMouseY();

		if (SDL_PointInRect(&temp, &launch_button.active_area) && game_state != PAUSE) {
			p2List_item<PhysBody*>* cir = circles.getFirst();

			while (cir != NULL)
			{
				if (cir->data->Contains(752, 735))
				{
					launch_button.ChangeState();
					cir->data->body->ApplyForce({ 0.0f, -150.0f }, cir->data->body->GetPosition(), true);
					App->audio->PlayFx(launcher_fx);

					if (game_state == START)game_state = IN_GAME;
				}

				cir = cir->next;
			}


		}

		if ((SDL_PointInRect(&temp, &new_game_button.active_area) && game_state == NO_GAME) || (SDL_PointInRect(&temp, &new_game_button.active_area) && game_state == END_GAME)) {

			new_game_button.ChangeState();
			game_state = START;

			ball_body = App->physics->CreateCircle(752, 725, 10, BALL);
			circles.add(ball_body);
			circles.getLast()->data->listener = this;

			Balls_count = 1;
			App->player->stand_lives = 3;
		}

		if (SDL_PointInRect(&temp, &volume_button.active_area) && game_state != PAUSE) {

			volume_button.ChangeState();
			if (volume_button.current_texture == volume_button.off_texture) {

				max_volume = Mix_VolumeMusic(0);
				Mix_Volume(-1, 0);

			}
			else {

				Mix_Volume(-1, max_volume);
				Mix_VolumeMusic(max_volume);

			}

		}

		if (SDL_PointInRect(&temp, &pause_rect) && game_state != PAUSE) {
			last_game_state = game_state;
			game_state = PAUSE;

		}

		if (SDL_PointInRect(&temp, &unpause_rect) && game_state == PAUSE) {
			game_state = last_game_state;
		}

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

	


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	

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

	// All draw functions ------------------------------------------------------
	//wheeels
	int pos_x, pos_y;
	int wheel_time = SDL_GetTicks();

	if (collide_wheel)
	{
		time_spinning = wheel_time;
		collide_wheel = false;
	}
	if (wheel_time > time_spinning + 500)
	{
		for (int i = 0; i < 3; i++)
		{
			wheels_on[i] = false;
		}
	}


	App->physics->mid_wheel->GetPosition(pos_x, pos_y);
	if (wheels_on[0])
		App->renderer->Blit(wheel, pos_x - 15, pos_y - 15, NULL, 1.0f, App->physics->mid_wheel->GetRotation());
	else
	{
		App->renderer->Blit(wheel_off, pos_x - 15, pos_y - 15, NULL, 1.0f, App->physics->mid_wheel->GetRotation());
		App->physics->mid_wheel->joint->EnableMotor(false);
		App->physics->mid_wheel->body->SetAngularVelocity(0.0f);
	}

	App->physics->left_wheel->GetPosition(pos_x, pos_y);
	if (wheels_on[1])
		App->renderer->Blit(wheel, pos_x - 15, pos_y - 15, NULL, 1.0f, App->physics->left_wheel->GetRotation());
	else
	{
		App->renderer->Blit(wheel_off, pos_x - 15, pos_y - 15, NULL, 1.0f, App->physics->left_wheel->GetRotation());
		App->physics->left_wheel->joint->EnableMotor(false);
		App->physics->left_wheel->body->SetAngularVelocity(0.0f);
	}


	App->physics->right_wheel->GetPosition(pos_x, pos_y);
	if (wheels_on[2])
		App->renderer->Blit(wheel, pos_x - 15, pos_y - 15, NULL, 1.0f, App->physics->right_wheel->GetRotation());
	else
	{
		App->renderer->Blit(wheel_off, pos_x - 15, pos_y - 15, NULL, 1.0f, App->physics->right_wheel->GetRotation());
		App->physics->right_wheel->joint->EnableMotor(false);
		App->physics->right_wheel->body->SetAngularVelocity(0.0f);
	}

	


	//All superior Renders

	//Blit the wheel flames
	App->renderer->Blit(wheel_flames_1, 425, 54);
	App->renderer->Blit(wheel_flames_2, 585, 95);
	App->renderer->Blit(wheel_flames_3, 474, 228);


	//All superior Renders
	
	//App->renderer->Blit(scape_light_4, 182, 318);
	//App->renderer->Blit(scape_light_5, 110, 193);

	App->renderer->Blit(left_lung, 286, 588);
	App->renderer->Blit(right_lung, 530, 582);

	//Blit the launcher
	/*
	App->renderer->Blit(launcher, 655, 450);
	App->renderer->Blit(scape_1, 158, 91);
	App->renderer->Blit(scape_2, 121, 16);

	App->renderer->Blit(scape_light_2, 412, 12);
	App->renderer->Blit(scape_light_3, 148, 70);

	App->renderer->Blit(down_yellow_light, 352, 363);
	App->renderer->Blit(down_orange_light, 335, 400);
	App->renderer->Blit(down_red_light, 324, 436);

	App->renderer->Blit(mid_red_light, 308, 252);
	App->renderer->Blit(mid_orange_light, 333, 212);
	App->renderer->Blit(mid_yellow_light, 357, 178);

	App->renderer->Blit(up_light_1, 496, 77);
	App->renderer->Blit(up_light_2, 521, 79);
	App->renderer->Blit(up_light_3, 548, 84);
	*/

	//App->renderer->Blit(pause, 0, 0);
	//App->renderer->Blit(scape_light_6, 215, 6);

	App->renderer->Blit(launch_button.current_texture, 846, 716);
	App->renderer->Blit(new_game_button.current_texture, 943, 717);
	App->renderer->Blit(volume_button.current_texture, 832, 525);
	
	App->renderer->Blit(hot_rod_wheels_light, 445, 165);
	//App->renderer->Blit(instructions, 382, 460);


	if(scape_light_1_on)
		App->renderer->Blit(scape_light_1, 620, 354);
	if (scape_light_2_on)
		App->renderer->Blit(scape_light_2, 412, 12);
	if (scape_light_3_on)
		App->renderer->Blit(scape_light_3, 148, 70);
	if (scape_light_4_on)
		App->renderer->Blit(scape_light_4, 182, 318);
	if (scape_light_5_on)
		App->renderer->Blit(scape_light_5, 110, 193);
	if (scape_light_6_on)
		App->renderer->Blit(scape_light_6, 215, 6);

	if(down_yellow_light_on)
		App->renderer->Blit(down_yellow_light, 352, 363);
	if (down_orange_light_on)
		App->renderer->Blit(down_orange_light, 335, 400);
	if (down_red_light_on)
		App->renderer->Blit(down_red_light, 324, 436);
	if (up_yellow_light_on)
		App->renderer->Blit(mid_yellow_light, 357, 178);
	if (up_orange_light_on)
		App->renderer->Blit(mid_orange_light, 333, 212);
	if (up_red_light_on)
		App->renderer->Blit(mid_red_light, 308, 252);
	
	if (up_lights[0])
		App->renderer->Blit(up_light_1, 496, 77);
	if (up_lights[1])
		App->renderer->Blit(up_light_2, 521, 79);
	if (up_lights[2])
		App->renderer->Blit(up_light_3, 548, 84);


	App->physics->flap_down_left->GetPosition(pos_x, pos_y);
	App->renderer->Blit(left_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_down_left->GetRotation());

	App->physics->flap_down_right->GetPosition(pos_x, pos_y);
	App->renderer->Blit(right_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_down_right->GetRotation());

	App->physics->flap_up_left->GetPosition(pos_x, pos_y);
	App->renderer->Blit(left_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_up_left->GetRotation());

	App->physics->flap_up_right->GetPosition(pos_x, pos_y);
	App->renderer->Blit(right_flap, pos_x - 6, pos_y - 5, NULL, 1.0f, App->physics->flap_up_right->GetRotation());

	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (game_state == PAUSE) {
		App->renderer->Blit(pause, 0, 0);
	}

	if (game_state == START) {
		App->renderer->Blit(instructions, 382, 460);
	}

	if (game_state == NO_GAME || game_state == END_GAME) {

		new_game_button.current_texture = new_game_button.on_texture;
		App->renderer->Blit(end_phrase, 350, 400);
	}
	else new_game_button.current_texture = new_game_button.off_texture;

	if (game_state == END_GAME) 
	{
		Balls_count = 0;
		App->player->extra_balls = 0;
	}

	return UPDATE_CONTINUE;
}


update_status ModuleSceneIntro::PostUpdate()
{
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
	App->physics->If_Sensor_contact(bodyA, bodyB);
	if (bodyB->body == App->physics->left_wheel->body || bodyB->body == App->physics->mid_wheel->body || bodyB->body == App->physics->right_wheel->body)
		App->physics->If_wheel_contact(bodyA, bodyB);
	
}

void ModuleSceneIntro::switch_up_lights()
{
	bool temp = up_lights[2];

	for (int i = 2; i > 0; i--)
		up_lights[i] = up_lights[i - 1];

	up_lights[0] = temp;
}

void ModuleSceneIntro::Set_lights_to_false()
{

	down_yellow_light_on = false;
	down_orange_light_on = false;
	down_red_light_on = false;
	up_yellow_light_on = false;
	up_orange_light_on = false;
	up_red_light_on = false;

	for (int i = 0; i < 3; i++)
	{
		up_lights[i] = false;
	}


}