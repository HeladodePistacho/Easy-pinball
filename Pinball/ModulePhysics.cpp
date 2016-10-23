#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"

#ifdef _DEBUG
#pragma comment( lib, "Engine/Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Engine/Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	flap_down_left_fix_joint = NULL;
	debug = true;
	name.create("physics");
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Awake(pugi::xml_node& config) {

	LOG("ModulePhysics Configuration Loaded!");
	return true;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	//Flaps
	flap_down_right = App->physics->CreateRectangle(535, 784, 70, 12, MAP);
	flap_down_left = App->physics->CreateRectangle(388, 783, 70, 12, MAP);
	flap_up_left = App->physics->CreateRectangle(300, 309, 70, 12, MAP);
	flap_up_right = App->physics->CreateRectangle(630, 375, 70, 12, MAP);

	//Flaps points
	flap_down_right_point = App->physics->CreateStaticCircle(535, 784, 4);
	flap_down_left_point = App->physics->CreateStaticCircle(388, 783, 4);
	flap_up_left_point = App->physics->CreateStaticCircle(300, 309, 4);
	flap_up_right_point = App->physics->CreateStaticCircle(630, 375, 4);

	//Flaps joints
	b2RevoluteJointDef def;

	//Up Right
	def.bodyA = flap_up_right_point->body;
	def.bodyB = flap_up_right->body;
	def.lowerAngle = -55 * DEGTORAD;
	def.upperAngle = -20 * DEGTORAD;
	def.enableLimit = true;
	def.localAnchorB = { 0.500f,0.0f };
	flap_up_right_fix_joint = (b2RevoluteJoint*)world->CreateJoint(&def);

	//Down Right
	def.bodyA = flap_down_right_point->body;
	def.bodyB = flap_down_right->body;
	def.lowerAngle = -22 * DEGTORAD;
	def.upperAngle = 30 * DEGTORAD;
	def.enableLimit = true;
	def.localAnchorB = { 0.500f,0.0f };
	flap_down_right_fix_joint = (b2RevoluteJoint*)world->CreateJoint(&def);

	//Up Left
	def.bodyA = flap_up_left_point->body;
	def.bodyB = flap_up_left->body;
	def.lowerAngle = -10 * DEGTORAD;
	def.upperAngle = 35 * DEGTORAD;
	def.enableLimit = true;
	def.localAnchorB = { -0.500f,0.0f };
	flap_up_left_fix_joint = (b2RevoluteJoint*)world->CreateJoint(&def);

	//Down Left
	def.bodyA = flap_down_left_point->body;
	def.bodyB = flap_down_left->body;
	def.lowerAngle = -26 * DEGTORAD;
	def.upperAngle = 24 * DEGTORAD;
	def.enableLimit = true;
	def.localAnchorB = { -0.500f,0.0f };
	flap_down_left_fix_joint = (b2RevoluteJoint*)world->CreateJoint(&def);

	//Wheels

	//Mid
	mid_wheel_point = App->physics->CreateStaticCircle(532, 242, 4);
	mid_wheel = CreateCircle(532, 238, 34, MAP);
	def.bodyA = mid_wheel->body;
	def.bodyB = mid_wheel_point->body;
	def.motorSpeed = -8.0f;
	def.maxMotorTorque = 80.0f;
	def.localAnchorB = { 0.0f , 0.0f };
	def.enableMotor = true;
	def.enableLimit = false;
	mid_wheel_engine = (b2RevoluteJoint*)world->CreateJoint(&def);

	//Left
	left_wheel_point = App->physics->CreateStaticCircle(474, 180, 4);
	left_wheel = CreateCircle(474, 180, 34, MAP);
	def.bodyA = left_wheel->body;
	def.bodyB = left_wheel_point->body;
	def.motorSpeed = -8.0f;
	def.maxMotorTorque = 80.0f;
	def.localAnchorB = { 0.0f , 0.0f };
	def.enableMotor = true;
	def.enableLimit = false;
	left_wheel_engine = (b2RevoluteJoint*)world->CreateJoint(&def);

	//Right
	right_wheel_point = App->physics->CreateStaticCircle(590, 180, 4);
	right_wheel = CreateCircle(590, 180, 34, MAP);
	def.bodyA = right_wheel->body;
	def.bodyB = right_wheel_point->body;
	def.motorSpeed = -8.0f;
	def.maxMotorTorque = 80.0f;
	def.localAnchorB = { 0.0f , 0.0f };
	def.enableMotor = true;
	def.enableLimit = false;
	mid_wheel_engine = (b2RevoluteJoint*)world->CreateJoint(&def);


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}



	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, collision_type type)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = type;
	fixture.filter.maskBits = LAUNCHER | DOOR_SENSOR;

	b->CreateFixture(&fixture);
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateStaticCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, collision_type type)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = type;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, collision_type type)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.categoryBits = type;
	fixture.filter.maskBits = BALL;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, collision_type type, uint restitution)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.categoryBits = type;
	fixture.filter.maskBits = BALL;
	
	fixture.restitution = restitution;
	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	bool body_clicked = false;
	b2Body* click_body = nullptr;

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{

				if (f->TestPoint({ PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) }))
				{
					body_clicked = true;
					click_body = f->GetBody();
				}
			 }
			
		}
	}

	b2MouseJointDef def;

	if (body_clicked)
	{	
		def.bodyA = ground;
		def.bodyB = click_body;
		def.target = {PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())};
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * click_body->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);

	}
	
	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		mouse_joint->SetTarget({ PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) });
		App->renderer->DrawLine((App->input->GetMouseX()), (App->input->GetMouseY()), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 0, 0);
	}

	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}

	return UPDATE_CONTINUE;
}

void ModulePhysics::PushUpLeftFlaps() {

	flap_down_left->body->ApplyForce({ -12,0 }, { 70,0 }, true);
	flap_up_left->body->ApplyForce({ -12,0 }, { 70,0 }, true);

}

void ModulePhysics::PushUpRightFlaps() {

	flap_up_right->body->ApplyForce({ 12,0 }, { 0,0 }, true);
	flap_down_right->body->ApplyForce({ 12,0 }, { 0,0 }, true);

}
void ModulePhysics::PushDownLeftFlaps() {

	flap_down_left->body->ApplyForce({ 12,0 }, { 70,0 }, true);
	flap_up_left->body->ApplyForce({ 12,0 }, { 70,0 }, true);

}

void ModulePhysics::PushDownRightFlaps() {

	flap_up_right->body->ApplyForce({ -12,0 }, { 0,0 }, true);
	flap_down_right->body->ApplyForce({ -12,0 }, { 0,0 }, true);

}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::If_Sensor_contact(PhysBody* bodyA, PhysBody* bodyB)
{

	b2Filter filter = bodyA->body->GetFixtureList()->GetFilterData();

	switch (bodyB->body->GetFixtureList()->GetFilterData().categoryBits)
	{
		case DOOR_SENSOR:
			filter.maskBits = MAP | SENSOR_RAMP_C | SENSOR_RAMP_B | SENSOR_RAMP_A | TURBO_SENSOR_UP | TURBO_SENSOR_DOWN | YELLOW_LIGHT | ORANGE_LIGHT | RED_LIGHT;
			bodyA->body->GetFixtureList()->SetFilterData(filter);
			break;
		
		case SENSOR_RAMP_A:
			
			filter.maskBits = RAMP_A | FINAL_RAMP_SENSOR | TURBO_SENSOR_UP;
			bodyA->body->GetFixtureList()->SetFilterData(filter);
			App->audio->PlayFx(App->scene_intro->drift_1_fx);
			break;

		case SENSOR_RAMP_B:
			
			filter.maskBits = RAMP_B | FINAL_RAMP_SENSOR | TURBO_SENSOR_UP;
			bodyA->body->GetFixtureList()->SetFilterData(filter);
			App->audio->PlayFx(App->scene_intro->special_ramp_fx);
			break;

		case SENSOR_RAMP_C:
			bodyA->body->ApplyForce({ -50.0f, 0.0f }, bodyA->body->GetPosition(), true);
			filter.maskBits = RAMP_C | FINAL_RAMP_SENSOR;
			bodyA->body->GetFixtureList()->SetFilterData(filter);
			App->audio->PlayFx(App->scene_intro->drift_1_fx);
			break;

		case FINAL_RAMP_SENSOR:

			for (int i = 0; i < 200000; i++)
			{
				bodyA->body->SetLinearVelocity({ 0.0f,0.0f });
			}
			filter.maskBits = MAP | SENSOR_RAMP_C | SENSOR_RAMP_B | SENSOR_RAMP_A | TURBO_SENSOR_UP | TURBO_SENSOR_DOWN;

			if (!App->scene_intro->down_yellow_light_on)
				filter.maskBits = filter.maskBits | YELLOW_LIGHT;
			if (!App->scene_intro->down_orange_light_on)
				filter.maskBits = filter.maskBits | ORANGE_LIGHT;
			if (!App->scene_intro->down_red_light_on)
				filter.maskBits = filter.maskBits | RED_LIGHT;


			bodyA->body->GetFixtureList()->SetFilterData(filter);
			break;

		case TURBO_SENSOR_UP:
			bodyA->body->ApplyForce({ 0.0f, -200.0f }, bodyA->body->GetPosition(), true);
			break;

		case TURBO_SENSOR_DOWN:
			bodyA->body->ApplyForce({ 0.0f, 50.0f }, bodyA->body->GetPosition(), true);
			break;

		case YELLOW_LIGHT:
			bodyA->body->SetLinearVelocity({ 0.0f,0.0f });
			filter.maskBits = MAP | SENSOR_RAMP_C | SENSOR_RAMP_B | SENSOR_RAMP_A | TURBO_SENSOR_UP | TURBO_SENSOR_DOWN;
			App->scene_intro->down_yellow_light_on = true;
			break;

		case ORANGE_LIGHT:
			bodyA->body->SetLinearVelocity({ 0.0f,0.0f });
			filter.maskBits = MAP | SENSOR_RAMP_C | SENSOR_RAMP_B | SENSOR_RAMP_A | TURBO_SENSOR_UP | TURBO_SENSOR_DOWN;
			App->scene_intro->down_orange_light_on = true;
			break;

		case RED_LIGHT:
			bodyA->body->SetLinearVelocity({ 0.0f,0.0f });
			filter.maskBits = MAP | SENSOR_RAMP_C | SENSOR_RAMP_B | SENSOR_RAMP_A | TURBO_SENSOR_UP | TURBO_SENSOR_DOWN;
			App->scene_intro->down_red_light_on = true;
			break;

	}


}