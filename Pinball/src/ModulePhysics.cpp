#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 8, 3);

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

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, bool dynamic_body)
{
	x = x * SCREEN_SIZE;
	y = y * SCREEN_SIZE;
	b2BodyDef body;
	if (dynamic_body == true) { body.type = b2_dynamicBody;}
	else if (dynamic_body == false) { body.type = b2_staticBody; }
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	if (body.type == b2_dynamicBody) { fixture.friction = 0.0f; }

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}
PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius)
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
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}
PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, bool dynamic_body)
{
	x = x * SCREEN_SIZE;
	y = y * SCREEN_SIZE;
	width = width * SCREEN_SIZE;
	height = height * SCREEN_SIZE;

	b2BodyDef body;
	if (dynamic_body == true) { body.type = b2_dynamicBody; }
	else if (dynamic_body == false) { body.type = b2_staticBody; }
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	x = x * SCREEN_SIZE;
	y = y * SCREEN_SIZE;
	width = width * SCREEN_SIZE;
	height = height * SCREEN_SIZE;

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

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateFlicker(PhysBody flicker, bool flip) {
	b2RevoluteJointDef def;
	b2RevoluteJoint* revolute_joint_aux;
	b2Vec2 offset(0, 0);
	if (flip == false) { offset.x = PIXEL_TO_METERS(7); offset.y = 0; }
	else if(flip == true) { offset.x = PIXEL_TO_METERS(75); offset.y = 0; }

	def.Initialize(ground, flicker.body, flicker.body->GetWorldCenter()+offset);
	def.enableMotor = true;
	def.maxMotorTorque = 0.0f;
	def.motorSpeed = 90.0f * DEGTORAD;

	revolute_joint_aux = (b2RevoluteJoint*)world->CreateJoint(&def);

	return revolute_joint_aux;
}


PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, bool dynamic_body)
{
	x = x * SCREEN_SIZE;
	y = y * SCREEN_SIZE;

	b2BodyDef body;
	if (dynamic_body == true) { body.type = b2_dynamicBody; }
	else if (dynamic_body == false) { body.type = b2_staticBody; }
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0] * SCREEN_SIZE);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1] * SCREEN_SIZE);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}
PhysBody* ModulePhysics::CreateChainSensor(int x, int y, int* points, int size)
{
	x = x * SCREEN_SIZE;
	y = y * SCREEN_SIZE;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0] * SCREEN_SIZE);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1] * SCREEN_SIZE);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.isSensor = true;

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

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
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
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 100, 100);
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
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
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
			// Creates the mouse joint if the mouse's left button is pressed
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
				b2Vec2 mouse_position(PIXEL_TO_METERS(App->input->GetMouseX()) * SCREEN_SIZE, PIXEL_TO_METERS(App->input->GetMouseY()) * SCREEN_SIZE);
				if (App->scene_intro->player_circle->body->GetFixtureList()->GetShape()->TestPoint(App->scene_intro->player_circle->body->GetTransform(), mouse_position) == true && App->scene_intro->player_circle->body == f->GetBody()) {
					b2MouseJointDef def;
					def.bodyA = ground;
					def.bodyB = f->GetBody();
					def.target = mouse_position;
					def.dampingRatio = 2.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * f->GetBody()->GetMass();
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}
		}
	}

	// Draws and updates the mouse joint as long as the mouse button is kept pressed
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != NULL && mouse_joint->IsActive() == true) {
		b2Vec2 mouse_position(PIXEL_TO_METERS(App->input->GetMouseX()) * SCREEN_SIZE, PIXEL_TO_METERS(App->input->GetMouseY()) * SCREEN_SIZE);
		App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), METERS_TO_PIXELS(mouse_position.x), METERS_TO_PIXELS(mouse_position.y), 255, 255, 0);
		mouse_joint->SetTarget(mouse_position);
	}

	// Deletes mouse joint if mouse button is released
	if ((App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != NULL && mouse_joint->IsActive() == true)) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = NULL;
	}
	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
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
			// do we want the normal ?

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
	int i = 0;



	for (p2List_item<PhysBody*>* iterator = App->scene_intro->hundred_scoreSensors.getFirst(); iterator != NULL; iterator = iterator->next)
	{
		if (physA == iterator->data)
		{
			App->scene_intro->ScoreUpdater(100);
		}
	}

	for (p2List_item<PhysBody*>* iterator = App->scene_intro->bumperSensors.getFirst(); iterator != NULL; iterator = iterator->next)
	{
		if (physA == iterator->data)
		{
			App->audio->PlayFx(App->scene_intro->bumper_hit);
			App->scene_intro->ScoreUpdater(100);
		}
	}

	if (physA == App->scene_intro->square1)
	{
		App->audio->PlayFx(App->scene_intro->yellowSquare_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->sqcollected1 = true;
	}
	
	if (physA == App->scene_intro->square2)
	{
		App->audio->PlayFx(App->scene_intro->yellowSquare_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->sqcollected2 = true;
	}
	
	if (physA == App->scene_intro->square3)
	{
		App->audio->PlayFx(App->scene_intro->yellowSquare_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->sqcollected3 = true;
	}
	
	if (physA == App->scene_intro->square4)
	{
		App->audio->PlayFx(App->scene_intro->yellowSquare_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->sqcollected4 = true;
	}


	if (physA == App->scene_intro->tag1)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected1 = true;
	}
	
	if (physA == App->scene_intro->tag2)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected2 = true;
	}
	
	if (physA == App->scene_intro->tag3)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected3 = true;
	}
	
	if (physA == App->scene_intro->tag4)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected4 = true;
	}
	
	if (physA == App->scene_intro->tag5)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected5 = true;
	}
	
	if (physA == App->scene_intro->tag6)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected6 = true;
	}
	
	if (physA == App->scene_intro->tag7)
	{
		App->audio->PlayFx(App->scene_intro->tags_hit);
		App->scene_intro->ScoreUpdater(200);
		App->scene_intro->tagCollected7 = true;
	}

	if (physA == App->scene_intro->tenSensor)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(500);
		App->scene_intro->reverse10 = true;
		App->scene_intro->awardcount = App->scene_intro->awardcount+1;
	}
	if (physA == App->scene_intro->JSensor)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(500);
		App->scene_intro->reverseJ = true;
		App->scene_intro->awardcount += 1;
	}
	if (physA == App->scene_intro->JSensor)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(500);
		App->scene_intro->reverseJ = true;
		App->scene_intro->awardcount += 1;
	}
	if (physA == App->scene_intro->QSensor)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(500);
		App->scene_intro->reverseQ = true;
		App->scene_intro->awardcount += 1;
	}
	if (physA == App->scene_intro->KSensor)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(500);
		App->scene_intro->reverseK = true;
		App->scene_intro->awardcount += 1;
	}
	if (physA == App->scene_intro->AsSensor)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(500);
		App->scene_intro->reverseAs = true;
	}
	
	if (physA == App->scene_intro->light1)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected1 = true;
	}
	if (physA == App->scene_intro->light2)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected2 = true;
	}
	if (physA == App->scene_intro->light3)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected3 = true;
	}
	if (physA == App->scene_intro->light4)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected4 = true;
	}
	if (physA == App->scene_intro->light5)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected5 = true;
	}
	if (physA == App->scene_intro->light6)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected6 = true;
	}
	if (physA == App->scene_intro->light7)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected7 = true;
	}
	if (physA == App->scene_intro->light8)
	{
		App->audio->PlayFx(App->scene_intro->egg_hit);
		App->scene_intro->ScoreUpdater(100);
		App->scene_intro->collected8 = true;
	}

	
	if (physA == App->scene_intro->pulsator)
	{
		App->audio->PlayFx(App->scene_intro->card_hit);
		App->scene_intro->ScoreUpdater(App->scene_intro->pulsatorUP);
		App->scene_intro->pulsatorUP += 100u;

		if (App->scene_intro->pulsatorUP > 1000u)
		{
			App->scene_intro->pulsatorUP = 1000u;
		}
			
	}
	

	for (p2List_item<PhysBody*>* iterator = App->scene_intro->fivehundred_scoreSensors.getFirst(); iterator != NULL; iterator = iterator->next)
	{
		if (physA == iterator->data)
		{
			App->audio->PlayFx(App->scene_intro->numeric_hit);
			App->scene_intro->ScoreUpdater(500);
		}
	}

	for (p2List_item<PhysBody*>* iterator = App->scene_intro->thousand_scoreSensors.getFirst(); iterator != NULL; iterator = iterator->next)
	{
		if (physA == iterator->data)
		{
			App->audio->PlayFx(App->scene_intro->numeric_hit);
			App->scene_intro->ScoreUpdater(1000);
		}
	}

	if (physA == App->scene_intro->loseSensor)
	{
		App->scene_intro->ball -= 1u;
		App->scene_intro->previousscore = App->scene_intro->score;
		App->scene_intro->score = 0u;
		App->scene_intro->pulsatorUP = 100u;
		App->scene_intro->awardcount=0;

		App->scene_intro->reverse10 = false;
		App->scene_intro->reverseJ = false;
		App->scene_intro->reverseQ = false;
		App->scene_intro->reverseK = false;
		App->scene_intro->reverseAs = false;

		App->scene_intro->collected1 = false;
		App->scene_intro->collected2 = false;
		App->scene_intro->collected3 = false;
		App->scene_intro->collected4 = false;
		App->scene_intro->collected5 = false;
		App->scene_intro->collected6 = false;
		App->scene_intro->collected7 = false;
		App->scene_intro->collected8 = false;

		App->scene_intro->tagCollected1 = false;
		App->scene_intro->tagCollected2 = false;
		App->scene_intro->tagCollected3 = false;
		App->scene_intro->tagCollected4 = false;
		App->scene_intro->tagCollected5 = false;
		App->scene_intro->tagCollected6 = false;
		App->scene_intro->tagCollected7 = false;

		App->scene_intro->sqcollected1 = false;
		App->scene_intro->sqcollected2 = false;
		App->scene_intro->sqcollected3 = false;
		App->scene_intro->sqcollected4 = false;

		if (App->scene_intro->ball != 0){
			App->scene_intro->generate_player = true;
		}
		else {
			App->scene_intro->stage = ST_TITLE_SCREEN;
			App->scene_intro->ball = 02;
			App->scene_intro->score = 00;
		}
	}

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::EndContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::MouseJointDestroy() {	// This prevents crash when mouse joint exists and map is changed
	if (mouse_joint != NULL) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = NULL;
	}
}
