#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "Globals.h"

struct Bouncer
{
	Bouncer() : body(NULL), texture(NULL), hit_timer(0), fx(0)
	{}

	PhysBody* body;
	SDL_Texture* texture;
	Uint32 hit_timer;
	uint fx;
};

enum lightTypes
{
	blue_box,
	yellow_box,
	red_box,
	pink_box,
	orange_box,
	green_box,
	green_rectangle,
	girl_light,
	boy_light,
	yellow_light
};

class ModuleSceneIntro;

struct Light
{
	Light() : body(NULL), texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	PhysBody* body;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> balls;
	p2List<PhysBody*> walls;
	PhysBody* flipperDL; // down left
	PhysBody* flipperDR; // down right
	PhysBody* flipperUL; // up left
	PhysBody* flipperUR; // up right
	PhysBody* flipperML; // mid left
	PhysBody* flipperMR; // mid right 
	PhysBody* spring;
	PhysBody* spring_wheel;
	PhysBody* ball_start;
	Bouncer bouncer_left_body;
	Bouncer bouncer_right_body;
	Bouncer green_bouncer1;
	Bouncer green_bouncer2;
	Bouncer grey_bouncer1;
	Bouncer grey_bouncer2;
	Bouncer grey_bouncer3;
	Bouncer grey_bouncer4;
	Bouncer grey_bouncer5;
	Bouncer grey_bouncer6;
	Bouncer grey_bouncer7;


	PhysBody* sensor;
	bool sensed;

	SDL_Texture* ball;
	SDL_Texture* table;
	SDL_Texture* flipperDL_texture;
	SDL_Texture* flipperDR_texture;
	SDL_Texture* flipperUL_texture;
	SDL_Texture* flipperUR_texture;
	SDL_Texture* spring_texture;
	SDL_Texture* grey_bouncer_texture;
	SDL_Texture* green_bouncer_texture;
	SDL_Texture* points_texture;
	SDL_Texture* green_cube_texture;
	SDL_Texture* blue_cube_texture;
	SDL_Texture* red_cube_texture;
	SDL_Texture* pink_cube_texture;
	SDL_Texture* orange_cube_texture;
	SDL_Texture* yellow_cube_texture;
	SDL_Texture* yellow_light_texture;
	SDL_Texture* girl_texture;
	SDL_Texture* boy_texture;
	SDL_Texture* green_rectangle_texture;

	uint bonus_fx;
	uint flipper_fx;
	uint spring_down_fx;
	uint spring_up_fx;
	uint grey_green_bouncer_fx;
	uint wall_fx;
	uint color_box_fx;
	uint char_touch_fx;
	uint side_bouncer_fx;
	uint yellow_light_fx;
	
	

	p2DynArray<Light> lights;

	PhysBody* player_lose;
	uint player_lose_fx;
	uint last_score = 0;
	uint score = 0;
	uint lives = 3;
	uint counter_box = 0;
};
