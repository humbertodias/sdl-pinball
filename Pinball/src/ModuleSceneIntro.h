#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "SDL.h"
#include "box2d/box2d.h"

enum stage_state {
	ST_UNKNOWN,
	ST_TITLE_SCREEN,
	ST_LOW_STAGE,
	ST_HIGH_STAGE,
	ST_BONUS_STAGE
};
enum plat_state
{
	ST_LEFT,
	ST_RIGHT,
};

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
// Blit functions
	void LowStageBlit();
	void HighStageBlit();
	void TitleBlit();
// Charge functions
	void ChargeLowStage();
	void ChargeHighStage();
	void CreateFlicker();
	void CreatePlayer(float x, float y);
//Score update
	void ScoreUpdater(uint s);
// Checks special one way colliders
	void CheckOneWayColliders();

public:
	SDL_Texture* scene;
	PhysBody* player_circle;
	PhysBody* circle1;
	PhysBody* circle2;
	PhysBody* flicker1;
	PhysBody* flicker2;
	b2RevoluteJoint* revolute_joint_left;
	b2RevoluteJoint* revolute_joint_right;
	SDL_Rect scene_rect;
	SDL_Rect elements_rect;
	stage_state stage;
	stage_state buffer_stage;
	bool generate_player = false;
	int title_cursor;
	int penguin_timer;
	bool penguin_switch;
	
	PhysBody* loseSensor;


	//elements for the pulsator
	PhysBody* pulsator;
	b2Vec2 p;

	//Moving platform
	SDL_Texture* plattform_tex;
	PhysBody* plat;
	void pinkPlatformUpdate();
	bool Rightlimit = false;
	bool Leftlimit = true;
	b2Vec2 pVel;
	plat_state pstate;

	//Cards
	SDL_Texture* card_reverse;
	SDL_Texture* card10;
	SDL_Texture* cardJ;
	SDL_Texture* cardQ;
	SDL_Texture* cardK;
	SDL_Texture* cardAs;

	PhysBody* tenSensor;
	PhysBody* JSensor;
	PhysBody* QSensor;
	PhysBody* KSensor;
	PhysBody* AsSensor;

	// to check if the card is reversed or not;
	bool reverse10 = false;
	bool reverseJ = false;
	bool reverseQ = false;
	bool reverseK = false;
	bool reverseAs = false;

	void CardBlit();

	//lights
	SDL_Texture* lights;

	PhysBody* light1;
	PhysBody* light2;
	PhysBody* light3;
	PhysBody* light4;
	PhysBody* light5;
	PhysBody* light6;
	PhysBody* light7;
	PhysBody* light8;
	
	bool collected1 = false;
	bool collected2 = false;
	bool collected3 = false;
	bool collected4 = false;
	bool collected5 = false;
	bool collected6 = false;
	bool collected7 = false;
	bool collected8 = false;
	
	int correction = 1;
	void LightBLit();

	//tags
	SDL_Texture* tag_tex1;
	SDL_Texture* tag_tex2;
	SDL_Texture* tag_tex3;
	SDL_Texture* tag_tex4;
	SDL_Texture* tag_tex5;
	SDL_Texture* tag_tex6;
	SDL_Texture* tag_tex7;

	PhysBody* tag1;
	PhysBody* tag2;
	PhysBody* tag3;
	PhysBody* tag4;
	PhysBody* tag5;
	PhysBody* tag6;
	PhysBody* tag7;

	bool tagCollected1 = false;
	bool tagCollected2 = false;
	bool tagCollected3 = false;
	bool tagCollected4 = false;
	bool tagCollected5 = false;
	bool tagCollected6 = false;
	bool tagCollected7 = false;

	void TagBlit();

	//yellow rectangles
	SDL_Texture* yellowSquare;
	
	PhysBody* square1;
	PhysBody* square2;
	PhysBody* square3;
	PhysBody* square4;

	bool sqcollected1 = false;
	bool sqcollected2 = false;
	bool sqcollected3 = false;
	bool sqcollected4 = false;

	void YellowSquaresBlit();

	
	//FX
	uint bonus_fx;
	uint bumper_hit;
	uint yellowSquare_hit;
	uint tags_hit;
	uint numeric_hit;
	uint card_hit;
	uint egg_hit;
	uint award;
	
	
	//fonts
	int font_name;
	int font2;
	int fontblue;
	int fontlightblue;

	//score related variables
	char score_text[10];
	uint score = 0u;

	char topscore_text[10];
	uint topscore = 0u;

	char previousscore_text[10];
	uint previousscore = 0u;

	char ball_text[10];
	uint ball = 02;

	bool ballAward=false;
	int  awardcount = 0;

	uint pulsatorUP = 100u;
	char pulsatorUP_text[10];

	//------SENSORS------//
			
	//score sensors
	p2List<PhysBody*> hundred_scoreSensors;
	
	p2List<PhysBody*> bumperSensors;
	p2List<PhysBody*> yellowSquareSensors;
	p2List<PhysBody*> tagSensors;
	p2List<PhysBody*> cardSensors;
	p2List<PhysBody*> variableSensors;
	PhysBody* pinkPlattformSensor;
	
	p2List<PhysBody*> fivehundred_scoreSensors;	
	p2List<PhysBody*> thousand_scoreSensors;
	
};
