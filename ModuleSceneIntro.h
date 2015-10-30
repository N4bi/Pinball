#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

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

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
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


	PhysBody* sensor;
	bool sensed;

	
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* ball;
	SDL_Texture* table;
	SDL_Texture* flipperDL_texture;
	SDL_Texture* flipperDR_texture;
	SDL_Texture* flipperUL_texture;
	SDL_Texture* flipperUR_texture;
	SDL_Texture* spring_texture;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
