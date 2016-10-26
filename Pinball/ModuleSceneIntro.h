#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

enum GAME_STATE {

	NO_GAME,
	START,
	IN_GAME,
	END_GAME,
	PAUSE

};

class Button {
public:

	Button(SDL_Texture* on_texture, SDL_Texture* off_texture, SDL_Rect* active_area, uint fx = 0, bool start_state = false ):on_texture(on_texture),off_texture(off_texture),active_area(*active_area),fx(fx){

		if (start_state)current_texture = on_texture;
		else current_texture = off_texture;

	};
	Button() {};
	~Button() {};

	SDL_Texture* current_texture;
	SDL_Texture* on_texture;
	SDL_Texture* off_texture;
	uint fx;
	SDL_Rect active_area;

	void ChangeState() {
		if (current_texture == on_texture)current_texture = off_texture;
		else current_texture = on_texture;
	};
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Awake(pugi::xml_node&);
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	GAME_STATE game_state;
	GAME_STATE last_game_state;

	Button new_game_button;
	Button launch_button;
	Button volume_button;
	uint max_volume;

	SDL_Rect pause_rect;
	SDL_Rect unpause_rect;

	//Score
	char score_text[10];
	int score_font = -1;

	//Ball Speed
	char speed_text[100];
	int speed_font = -1;

	//Number of Balls
	char balls_text[10];
	int balls_font = -1;
	int Balls_count = 1;

	//Debug
	char debug_text[100];
	int debug_font = -1;
	char temp_text[17];

	//timing
	char time_count_text[14];
	char time_text[6];
	int initial_time = 0;
	int crone_time = 0;
	int ratio = 1000;
	int seconds = 0;
	int decimes = 0;
	int minutes = 0;



public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* ball;
	PhysBody* ball_body;
	bool ball_into_jackpot = false;
	bool first_time = false;
	int stop_ball = 0;

	SDL_Texture* pause;
	SDL_Texture* instructions;


	SDL_Texture* launcher;
	SDL_Texture* scape_1;
	SDL_Texture* scape_2;

	SDL_Texture* scape_light_1;
	SDL_Texture* scape_light_2;
	SDL_Texture* scape_light_3;
	SDL_Texture* scape_light_4;
	SDL_Texture* scape_light_5;
	SDL_Texture* scape_light_6;
	
	bool scape_light_1_on = false;
	bool scape_light_2_on = false;
	bool scape_light_3_on = false;
	bool scape_light_4_on = false;
	bool scape_light_5_on = false;
	bool scape_light_6_on = false;

	SDL_Texture* wheel;
	SDL_Texture* wheel_off;

	SDL_Texture* wheel_flames_1;
	SDL_Texture* wheel_flames_2;
	SDL_Texture* wheel_flames_3;

	SDL_Texture* down_red_light;
	SDL_Texture* down_orange_light;
	SDL_Texture* down_yellow_light;

	SDL_Texture* mid_red_light;
	SDL_Texture* mid_orange_light;
	SDL_Texture* mid_yellow_light;

	bool down_yellow_light_on = false;
	bool down_orange_light_on = false;
	bool down_red_light_on = false;

	bool up_yellow_light_on = false;
	bool up_orange_light_on = false;
	bool up_red_light_on = false;

	SDL_Texture* up_light_1;
	SDL_Texture* up_light_2;
	SDL_Texture* up_light_3;

	bool up_lights[3] = { false, false, false };

	SDL_Texture* left_lung;
	SDL_Texture* right_lung;

	uint bonus_fx;
	uint flap_up_fx;
	uint flap_down_fx;
	uint launcher_fx;
	uint ramp_a_fx;

	uint drift_1_fx;
	uint drift_2_fx;
	uint special_ramp_fx;
	uint ping_fx;
	uint crash_1_fx;
	uint crash_2_fx;
	uint lung_boom_fx;
	uint ding_ding_fx;
	uint jackpot_fx;
	uint points_fx;
	uint wheels_1_fx;
	uint wheels_2_fx;

	p2Point<int> ray;
	bool ray_on;

private:

	SDL_Texture* background;
	SDL_Texture* left_flap;
	SDL_Texture* right_flap;
	void switch_up_lights();

	

};
