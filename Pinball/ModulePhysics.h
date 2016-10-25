#pragma once
#include "Module.h"
#include "Globals.h"
#include "Engine/Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -9.8f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum collision_type
{
	BALL = 1,
	RAMP_A = 2,
	RAMP_B = 4,
	RAMP_C = 8,
	MAP = 16,
	LAUNCHER = 32,
	SENSOR = 64,
	YELLOW_LIGHT = 128,
	ORANGE_LIGHT = 256,
	RED_LIGHT = 512,
	YELLOW_LIGHT_UP = 1024,
	ORANGE_LIGHT_UP = 2048,
	RED_LIGHT_UP = 4096,
	FINAL_RAMP = 8192
	
};

enum BODY_TYPE 
{
	NONE,
	SENSOR_RAMP_A,
	SENSOR_RAMP_B,
	SENSOR_RAMP_C,
	TURBO_UP,
	TURBO_UP_LEFT,
	TURBO_DOWN,
	WHEEL_LIGHT_LEFT,
	WHEEL_LIGHT_MID,
	WHEEL_LIGHT_RIGHT,
	END,
	JACKPOT,
	DOOR
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	

public:
	int width, height;
	b2Body* body;
	Module* listener;
	BODY_TYPE collide_type;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Awake(pugi::xml_node&);
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, collision_type type);
	PhysBody* CreateStaticCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height, collision_type type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, collision_type type, BODY_TYPE b_type = NONE);
	PhysBody* CreateChain(int x, int y, int* points, int size, collision_type type, uint restitution = 0);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void If_Sensor_contact(PhysBody* bodyA, PhysBody* bodyB);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

public:

	PhysBody* flap_up_right;
	PhysBody* flap_down_right;
	PhysBody* flap_up_left;
	PhysBody* flap_down_left;

	PhysBody* left_wheel;
	PhysBody* mid_wheel;
	PhysBody* right_wheel;

	void PushUpLeftFlaps();
	void PushUpRightFlaps();
	void PushDownLeftFlaps();
	void PushDownRightFlaps();


private:


	PhysBody* flap_up_right_point;
	PhysBody* flap_down_right_point;
	PhysBody* flap_up_left_point;
	PhysBody* flap_down_left_point;

	b2RevoluteJoint* flap_up_right_fix_joint;
	b2RevoluteJoint* flap_down_right_fix_joint;
	b2RevoluteJoint* flap_up_left_fix_joint;
	b2RevoluteJoint* flap_down_left_fix_joint;

	PhysBody* mid_wheel_point;
	PhysBody* left_wheel_point;
	PhysBody* right_wheel_point;

	b2RevoluteJoint* mid_wheel_engine;
	b2RevoluteJoint* left_wheel_engine;
	b2RevoluteJoint* right_wheel_engine;
};