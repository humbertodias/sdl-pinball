#include "Globals.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	left_flicker = false;
	right_flicker = false;
	kicker_timer = 0;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	player = App->textures->Load("pinball/player.png");
	player_fx = App->audio->LoadFx("pinball/audio/flicker.wav");
	kicker_fx = App->audio->LoadFx("pinball/audio/kicker.wav");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if(App->scene_intro->stage != ST_TITLE_SCREEN)
		App->renderer->Blit(player, position.x, position.y);

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PreUpdate() {
	
	// Stage calculation
	App->scene_intro->buffer_stage = App->scene_intro->stage;
	if (position.y < 0 && App->scene_intro->stage == ST_LOW_STAGE) {
		App->scene_intro->stage = ST_HIGH_STAGE;
	}
	if (position.y > SCREEN_HEIGHT && App->scene_intro->stage == ST_HIGH_STAGE) {
		App->scene_intro->stage = ST_LOW_STAGE;
	}
  	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->scene_intro->stage != ST_LOW_STAGE) {
		App->scene_intro->stage = ST_LOW_STAGE;
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && App->scene_intro->stage != ST_HIGH_STAGE) {
		App->scene_intro->stage = ST_HIGH_STAGE;
	}

	StateMachine();

	return UPDATE_CONTINUE;
}

void ModulePlayer::StateMachine() {
	if (App->scene_intro->stage == ST_TITLE_SCREEN) {
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && App->scene_intro->title_cursor != 4) {
			App->scene_intro->title_cursor++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && App->scene_intro->title_cursor != 1) {
			App->scene_intro->title_cursor--;
		}
	}
	// Controls
	if (App->scene_intro->player_circle != NULL) {
		speed = App->scene_intro->player_circle->body->GetLinearVelocity();
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) { kicker_timer++; }
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && speed.y == 0) {
			b2Vec2 kicker;
			kicker.y = -300;
			App->scene_intro->player_circle->body->ApplyForce(kicker, App->scene_intro->player_circle->body->GetWorldCenter(), true);
			kicker_timer = 0;
		} // Kicker
		if (App->scene_intro->buffer_stage == ST_LOW_STAGE && App->scene_intro->stage == ST_HIGH_STAGE) {
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		if (App->scene_intro->flicker1->body->GetAngle() > -45 * DEGTORAD) {
			App->scene_intro->flicker1->body->SetAngularVelocity(-15);
			left_flicker = true;
		}
		else { App->scene_intro->flicker1->body->SetAngularVelocity(0); }
	}	// LEFT FLIPPER
	else { left_flicker = false; }

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(player_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		if (App->scene_intro->flicker2->body->GetAngle() < 45 * DEGTORAD) {
			App->scene_intro->flicker2->body->SetAngularVelocity(15);
			right_flicker = true;
		}
		else { App->scene_intro->flicker2->body->SetAngularVelocity(0); }
	}	// RIGHT FLIPPER
	else { right_flicker = false; }

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(player_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
	}	// KICKER

	if (App->scene_intro->flicker1 != NULL && App->scene_intro->flicker2 != NULL) {
		if (left_flicker == false) {
			if (App->scene_intro->flicker1->body->GetAngle() < 0 * DEGTORAD) {
				App->scene_intro->flicker1->body->SetAngularVelocity(15);
			}
			else { App->scene_intro->flicker1->body->SetAngularVelocity(0); }
		}
		if (right_flicker == false) {
			if (App->scene_intro->flicker2->body->GetAngle() > 0 * DEGTORAD) {
				App->scene_intro->flicker2->body->SetAngularVelocity(-15);
			}
			else { App->scene_intro->flicker2->body->SetAngularVelocity(0); }
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		App->audio->PlayFx(kicker_fx);
	}

}
