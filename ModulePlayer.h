#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

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

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* flipperDL_texture;
	SDL_Texture* flipperDR_texture;
	SDL_Texture* flipperUL_texture;
	SDL_Texture* flipperUR_texture;
	SDL_Texture* spring_texture;

	uint bonus_fx;
	uint flipper_fx;
	uint spring_down_fx;
	uint spring_up_fx;

};