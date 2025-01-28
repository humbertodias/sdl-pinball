#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include <Box2D/Box2D.h>

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	void StateMachine();

public:
	int kicker_timer;
	b2Vec2 speed;
	iPoint position;
	iPoint buffer_position;
	bool left_flicker;
	bool right_flicker;
	SDL_Texture* player;
	uint player_fx;
	uint kicker_fx;
};