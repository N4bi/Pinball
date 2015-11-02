#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "ModulePlayer.h"

#define BOUNCE_TIME 300

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	table = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	
	LOG("Loading Intro assets");
	bool ret = true;
	App->renderer->camera.x = App->renderer->camera.y = 0;

	
	//Graphics 

			//Table

	table = App->textures->Load("Game/pinball/ground3.png");
	grey_bouncer_texture = App->textures->Load("Game/pinball/grey_bouncer.png");
	green_bouncer_texture = App->textures->Load("Game/pinball/green_bouncer.png");
	bouncer_left_texture = App->textures->Load("Game/pinball/side_bouncer_left.png");
	bouncer_right_texture = App->textures->Load("Game/pinball/side_bouncer_right.png");
	points_texture = App->textures->Load("Game/pinball/+100.png");

			//Sensor Lights
	green_cube_texture = App->textures->Load("Game/pinball/green.png");
	blue_cube_texture = App->textures->Load("Game/pinball/blue.png");
	red_cube_texture = App->textures->Load("Game/pinball/red.png");
	pink_cube_texture = App->textures->Load("Game/pinball/pink.png");
	orange_cube_texture = App->textures->Load("Game/pinball/orange.png");
	yellow_cube_texture = App->textures->Load("Game/pinball/yellow.png");
	yellow_light_texture = App->textures->Load("Game/pinball/yellow_light.png");
	girl_texture = App->textures->Load("Game/pinball/girl.png");
	boy_texture = App->textures->Load("Game/pinball/boy.png");
	green_rectangle_texture = App->textures->Load("Game/pinball/green_rectangle_light.png");


	//Audio

			//Music
	App->audio->PlayMusic("Game/pinball/sounds/music_pinball.ogg");
			
			//SFX 
				
				//Table
	grey_green_bouncer_fx = App->audio->LoadFx("Game/pinball/sounds/green_grey_bouncer_hit.wav");
	wall_fx = App->audio->LoadFx("Game/pinball/sounds/wall_hit.wav");

				//Sensor fx
	color_box_fx = App->audio->LoadFx("Game/pinball/sounds/color_box.wav");
	color_box_5_fx = App->audio->LoadFx("Game/pinball/sounds/color_box_5.wav");
	char_touch_fx = App->audio->LoadFx("Game/pinball/sounds/char_touch.wav");
	side_bouncer_fx = App->audio->LoadFx("Game/pinball/sounds/bell.wav");
	yellow_light_fx = App->audio->LoadFx("Game/pinball/sounds/dog_barking.wav");
	green_rectangle_fx = App->audio->LoadFx("Game/pinball/sounds/green_rectangle.wav");
	green_rectangle_2_fx = App->audio->LoadFx("Game/pinball/sounds/green_rectangle_2.wav");
	player_lose_fx = App->audio->LoadFx("Game/pinball/sounds/player_lose.wav");


/////////////////////////////////////////////////////////////////

	// Table vertex

	int triangle_right[44] = {
		343, 944,
		356, 968,
		374, 958,
		406, 940,
		434, 925,
		442, 919,
		448, 912,
		453, 901,
		456, 890,
		456, 873,
		454, 863,
		447, 872,
		444, 880,
		440, 886,
		433, 892,
		414, 904,
		400, 912,
		390, 917,
		380, 923,
		365, 931,
		354, 936,
		347, 941
	};

	int launcher[88] = {
		354, 1021,
	368, 1007,
	489, 942,
	490, 764,
	487, 748,
	482, 734,
	475, 718,
	465, 705,
	456, 696,
	440, 690,
	446, 674,
	454, 678,
	462, 684,
	475, 697,
	484, 710,
	489, 718,
	494, 728,
	497, 738,
	500, 749,
	501, 757,
	501, 764,
	501, 1022,
	531, 1022,
	531, 855,
	531, 757,
	530, 748,
	526, 733,
	521, 718,
	515, 706,
	504, 689,
	491, 675,
	479, 663,
	468, 656,
	456, 650,
	461, 640,
	478, 648,
	489, 656,
	502, 668,
	514, 683,
	529, 706,
	537, 728,
	542, 753,
	542, 1024,
	354, 1023
	};

	int table[70] = {
		33, 1020,
		33, 726,
		54, 725,
		58, 720,
		58, 652,
		53, 644,
		36, 644,
		33, 636,
		33, 596,
		31, 213,
		31, 60,
		52, 56,
		68, 34,
		71, 22,
		82, 19,
		206, 20,
		216, 46,
		238, 56,
		260, 44,
		267, 21,
		500, 20,
		507, 38,
		521, 50,
		538, 58,
		537, 196,
		537, 638,
		517, 653,
		517, 672,
		535, 682,
		535, 717,
		545, 756,
		545, 3,
		2, 2,
		1, 1021,
		30, 1022
	};
	int triangle_left[38] = {
		73, 862,
		69, 873,
		67, 885,
		69, 894,
		75, 905,
		79, 916,
		88, 924,
		103, 932,
		128, 944,
		150, 957,
		169, 967,
		173, 967,
		184, 945,
		172, 937,
		150, 926,
		117, 908,
		89, 891,
		81, 879,
		77, 864
	};

	int triangle_left2[12] = {
		177, 1022,
		160, 1007,
		42, 944,
		35, 947,
		35, 1023,
		172, 1023
	};

	int mid_triangle_left[12] = {
		87, 418,
		87, 565,
		116, 520,
		95, 480,
		111, 466,
		89, 420
	};

	int mid_triangle_right[18] = {
		483, 643,
		508, 657,
		521, 643,
		533, 639,
		534, 518,
		528, 518,
		497, 584,
		514, 597,
		487, 643
	};

	int rectangle_up_left[14] = {
		310, 172,
		317, 188,
		353, 211,
		344, 225,
		309, 205,
		297, 178,
		307, 172
	};

	int rectangle_up_right[18] = {
		525, 176,
		520, 185,
		515, 190,
		480, 211,
		490, 226,
		526, 206,
		536, 182,
		530, 165,
		526, 174
	};

	int triangle_up_left[20] = {
		295, 166,
		287, 161,
		249, 76,
		252, 66,
		268, 56,
		278, 33,
		300, 33,
		303, 39,
		303, 155,
		299, 163
	};

	int triangle2_up_left[8] = {
		360, 35,
		308, 82,
		308, 35,
		358, 35
	};

	int tunnel_wall_right[66] = {
		179, 124,
		175, 120,
		174, 104,
		166, 85,
		146, 70,
		126, 68,
		105, 76,
		91, 90,
		81, 112,
		80, 140,
		85, 568,
		88, 576,
		88, 584,
		106, 607,
		113, 623,
		113, 728,
		109, 733,
		104, 730,
		104, 627,
		98, 613,
		80, 589,
		78, 581,
		72, 140,
		72, 111,
		75, 96,
		85, 81,
		101, 67,
		125, 60,
		145, 62,
		162, 72,
		175, 87,
		184, 107,
		184, 123
	};

	int tunnel_wall_left[76] = {
		230, 117,
		228, 112,
		227, 94,
		213, 68,
		185, 46,
		168, 38,
		144, 32,
		126, 31,
		103, 34,
		77, 49,
		61, 64,
		46, 95,
		44, 109,
		46, 579,
		51, 597,
		60, 613,
		70, 626,
		73, 635,
		72, 728,
		68, 733,
		62, 728,
		63, 639,
		60, 628,
		52, 617,
		41, 600,
		35, 580,
		36, 108,
		40, 90,
		58, 61,
		79, 41,
		103, 28,
		126, 24,
		145, 24,
		171, 31,
		193, 43,
		218, 65,
		233, 90,
		235, 116
	};

	int multi_ball_stuff[46] = {
		90, 259,
		109, 204,
		102, 183,
		109, 179,
		113, 190,
		163, 162,
		156, 154,
		159, 147,
		156, 143,
		165, 135,
		170, 126,
		172, 109,
		163, 90,
		153, 80,
		134, 74,
		120, 77,
		106, 85,
		93, 106,
		81, 112,
		81, 119,
		88, 139,
		85, 144,
		85, 258
	};

	int bouncer_left[12] = {
		112, 771,
		125, 767,
		177, 855,
		167, 864,
		117, 840,
		112, 775
	};

	int bouncer_right[12] = {
		353, 854,
		362, 864,
		412, 839,
		419, 775,
		404, 770,
		355, 849
	};

/////////////////////////////////////////////////////////////////

	//Walls & Bouncers
	walls.add(App->physics->AddWall(0, 0, table, 70));

	walls.add(App->physics->AddWall(0, 0, triangle_right, 44,0.4f));
	walls.add(App->physics->AddWall(0, 0, launcher, 88, 0.0f));
	walls.add(App->physics->AddWall(0, 0, triangle_left, 38, 0.4f));
	walls.add(App->physics->AddWall(0, 0, triangle_left2, 12, 0.4f));
	walls.add(App->physics->AddWall(0, 0, mid_triangle_left, 12, 0.4f));
	walls.add(App->physics->AddWall(0, 0, mid_triangle_right, 18, 0.4f));
	walls.add(App->physics->AddWall(0, 0, rectangle_up_left, 14, 0.4f));
	walls.add(App->physics->AddWall(0, 0, rectangle_up_right, 18, 0.4f));
	walls.add(App->physics->AddWall(0, 0, triangle_up_left, 20, 0.4f));
	walls.add(App->physics->AddWall(0, 0, triangle2_up_left, 8, 0.4f));
	walls.add(App->physics->AddWall(0, 0, tunnel_wall_right, 66, 0.4f));
	walls.add(App->physics->AddWall(0, 0, tunnel_wall_left, 76, 0.4f));
	walls.add(App->physics->AddWall(0, 0, multi_ball_stuff, 46, 0.4f));



	// Other stuff

		//Side bouncers
	bouncer_left_body.body = App->physics->CreateChainStatic(0, 0, bouncer_left, 12, 1.0f, 0.7f, false);
	bouncer_left_body.body->listener = this;

	bouncer_right_body.body = App->physics->CreateChainStatic(0, 0, bouncer_right, 12, 1.0f, 0.7f, false);
	bouncer_right_body.body->listener = this;

		//Green bouncers on the middle of the table
	green_bouncer1.body = App->physics->CreateCircleStatic(464, 375, 32, 1.0f, 1.0f);
	green_bouncer1.body->listener = this;

	green_bouncer2.body = App->physics->CreateCircleStatic(322, 499, 32, 1.0f, 1.0f);
	green_bouncer2.body->listener = this;

		// Grey bouncers
	grey_bouncer1.body = App->physics->CreateCircleStatic(471, 756, 11, 1.0f, 0.6f);
	grey_bouncer1.body->listener = this;

	grey_bouncer2.body = App->physics->CreateCircleStatic(130, 298, 11, 1.0f, 0.6f);
	grey_bouncer2.body->listener = this;

	grey_bouncer3.body = App->physics->CreateCircleStatic(513, 248, 8, 1.0f, 0.6f);
	grey_bouncer3.body->listener = this;

	grey_bouncer4.body = App->physics->CreateCircleStatic(264, 148, 11, 1.0f, 0.6f);
	grey_bouncer4.body->listener = this;

	grey_bouncer5.body = App->physics->CreateCircleStatic(344, 144, 11, 1.0f, 0.6f);
	grey_bouncer5.body->listener = this;

	grey_bouncer6.body = App->physics->CreateCircleStatic(477, 144, 11, 1.0f, 0.6f);
	grey_bouncer6.body->listener = this;

	grey_bouncer7.body = App->physics->CreateCircleStatic(420, 79, 11, 1.0f, 0.6f);
	grey_bouncer7.body->listener = this;
	

///////////////////////////////////////////////////////////////

	//Sensors (all lights)
		
	lights.PushBack(Light(this, 366, 606, lightTypes::red_box));
	lights.PushBack(Light(this, 241, 577, lightTypes::pink_box));
	lights.PushBack(Light(this, 121, 601, lightTypes::yellow_box));
	lights.PushBack(Light(this, 172, 762, lightTypes::blue_box));
	lights.PushBack(Light(this, 286, 762, lightTypes::green_box));
	lights.PushBack(Light(this, 228, 869, lightTypes::orange_box));

	lights.PushBack(Light(this, 415, 764, lightTypes::girl_light));
	lights.PushBack(Light(this, 42, 767, lightTypes::boy_light));

	lights.PushBack(Light(this, 111, 545, lightTypes::yellow_light));
	lights.PushBack(Light(this, 97, 362, lightTypes::yellow_light));
	lights.PushBack(Light(this, 97, 314, lightTypes::yellow_light));
	lights.PushBack(Light(this, 112, 222, lightTypes::yellow_light));
	lights.PushBack(Light(this, 457, 468, lightTypes::yellow_light));
	lights.PushBack(Light(this, 457, 266, lightTypes::yellow_light));
	lights.PushBack(Light(this, 338, 60, lightTypes::yellow_light));
	lights.PushBack(Light(this, 441, 60, lightTypes::yellow_light));

	lights.PushBack(Light(this, 263, 296, lightTypes::green_rectangle));
	lights.PushBack(Light(this, 388, 117, lightTypes::green_rectangle));

	//Sensor for player losing
	
	player_lose = App->physics->CreateRectangleSensor(261, 1060, 200, 50);
	player_lose->listener = this;

	

	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(table);
	App->textures->Unload(green_bouncer_texture);
	App->textures->Unload(grey_bouncer_texture);
	App->textures->Unload(points_texture);
	App->textures->Unload(green_cube_texture);
	App->textures->Unload(blue_cube_texture);
	App->textures->Unload(red_cube_texture);
	App->textures->Unload(pink_cube_texture);
	App->textures->Unload(orange_cube_texture);
	App->textures->Unload(yellow_cube_texture);
	App->textures->Unload(yellow_light_texture);
	App->textures->Unload(girl_texture);
	App->textures->Unload(boy_texture);
	App->textures->Unload(green_rectangle_texture);
	App->textures->Unload(bouncer_left_texture);
	App->textures->Unload(bouncer_right_texture);




	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{ 

	//Draw stuff
	
	App->renderer->Blit(table, 0, 0);

	//Bouncer reactions stuff

			//Side Bouncers
	if(bouncer_right_body.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), bouncer_right_body.hit_timer) == false)
		{
			App->renderer->Blit(bouncer_right_texture, 401-55, 766);
		}
		else
		{
			bouncer_right_body.hit_timer = 0;
			score += 100;
		}
	}

	if (bouncer_left_body.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), bouncer_left_body.hit_timer) == false)
		{
			App->renderer->Blit(bouncer_left_texture, 111-5, 766);
		}
		else
		{
			bouncer_left_body.hit_timer = 0;
			score += 100;
		}
	}

		// Green Bouncers
	if (green_bouncer1.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), green_bouncer1.hit_timer) == false)
		{
			App->renderer->Blit(green_bouncer_texture, 464 - 50, 375 - 50);
		}
		else
		{
			green_bouncer1.hit_timer = 0;
			score += 100;
		}
	}

	if (green_bouncer2.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), green_bouncer2.hit_timer) == false)
		{
			App->renderer->Blit(green_bouncer_texture, 322 - 50, 499 - 50);
		}
		else
		{
			green_bouncer2.hit_timer = 0;
			score += 100;
		}
	}
		// Grey bouncers

	if (grey_bouncer1.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer1.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 471-11, 756-11);
			App->renderer->Blit(points_texture, 471-15, 756-15);
		}
		else
		{
			grey_bouncer1.hit_timer = 0;
			score += 100;
		}
	}

	if (grey_bouncer2.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer2.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 130-11, 298-11);
			App->renderer->Blit(points_texture, 130 - 15, 298 - 15);
		}
		else
		{
			grey_bouncer2.hit_timer = 0;
			score += 100;
		}
	}

	if (grey_bouncer3.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer3.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 510-11, 248-11);
			App->renderer->Blit(points_texture, 510 - 15, 248 - 15);
		}
		else
		{
			grey_bouncer3.hit_timer = 0;
			score += 100;
		}
	}

	if (grey_bouncer4.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer4.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 264-11, 148-11);
			App->renderer->Blit(points_texture, 264 - 15, 148 - 15);
		}
		else
		{
			grey_bouncer4.hit_timer = 0;
			score += 100;
		}
	}

	if (grey_bouncer5.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer5.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 344-11, 144-11);
			App->renderer->Blit(points_texture, 344 - 15, 144 - 15);
		}
		else
		{
			grey_bouncer5.hit_timer = 0;
			score += 100;
		}
	}

	if (grey_bouncer6.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer6.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 477-11, 144-11);
			App->renderer->Blit(points_texture, 477 - 15, 144 - 15);
		}
		else
		{
			grey_bouncer6.hit_timer = 0;
			score += 100;
		}
	}

	if (grey_bouncer7.hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), grey_bouncer7.hit_timer) == false)
		{
			App->renderer->Blit(grey_bouncer_texture, 420-11, 79-11);
			App->renderer->Blit(points_texture, 420 - 15, 79 - 15);
		}
		else
		{
			grey_bouncer7.hit_timer = 0;
			score += 100;
		}
	}

	for (uint i = 0; i < lights.Count(); ++i)
	{
		if (lights[i].on == true)
		{
			App->renderer->Blit(lights[i].texture, lights[i].x, lights[i].y);
			
		}
	}

	if (counter_box > 5)
	{
		score += 1000;

		for (uint i = 0; i < lights.Count(); ++i)
		{
			if (lights[i].on == true && lights[i].type == orange_box || lights[i].type == blue_box || lights[i].type == green_box || lights[i].type == red_box || lights[i].type == pink_box || lights[i].type == yellow_box)
			{
				lights[i].on = false;
				counter_box = 0;
				App->audio->PlayFx(color_box_5_fx);
				
			}
		}
	}

	if (counter_char_box > 1)
	{
		score += 1000;

		for (uint i = 0; i < lights.Count(); ++i)
		{
			if (lights[i].on == true && lights[i].type == boy_light || lights[i].type == girl_light)
			{
				lights[i].on = false;
				counter_char_box = 0;
				App->audio->PlayFx(char_touch_fx);
				

			}
		}
	}

	if (counter_yellow_lights > 7)
	{
		score += 5000;

		for (uint i = 0; i < lights.Count(); ++i)
		{
			if (lights[i].on == true && lights[i].type == yellow_light)
			{
				lights[i].on = false;
				counter_yellow_lights = 0;
				App->audio->PlayFx(char_touch_fx);

			}
		}
	}

	if (counter_green_rectangles > 1)
	{
		score += 1000;
		lives += 1;

		for (uint i = 0; i < lights.Count(); ++i)
		{
			if (lights[i].on == true && lights[i].type == green_rectangle)
			{
				lights[i].on = false;
				counter_green_rectangles = 0;
				App->audio->PlayFx(green_rectangle_2_fx);

			}
		}
	}


/////////////////////////////////////////////////////////////////

	char title[100];
	sprintf_s(title, "Balls: %d Score: %06d Last Score: %06d" , lives, score, last_score);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//Side Bouncers Collision
	if (bouncer_left_body.body == bodyA)
	{
		bouncer_left_body.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(side_bouncer_fx);
		return;
	}

	if (bouncer_right_body.body == bodyA)
	{
		bouncer_right_body.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(side_bouncer_fx);
		return;
	}


	//Green bouncer collision
	if (green_bouncer1.body == bodyA)
	{
		green_bouncer1.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}

	if (green_bouncer2.body == bodyA)
	{
		green_bouncer2.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}

	//Grey bouncer collision

	if (grey_bouncer1.body == bodyA)
	{
		grey_bouncer1.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}

	if (grey_bouncer2.body == bodyA)
	{
		grey_bouncer2.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}

	if (grey_bouncer3.body == bodyA)
	{
		grey_bouncer3.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}

	if (grey_bouncer4.body == bodyA)
	{
		grey_bouncer4.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}


	if (grey_bouncer5.body == bodyA)
	{
		grey_bouncer5.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}


	if (grey_bouncer6.body == bodyA)
	{
		grey_bouncer6.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}


	if (grey_bouncer7.body == bodyA)
	{
		grey_bouncer7.hit_timer = SDL_GetTicks() + BOUNCE_TIME;
		App->audio->PlayFx(grey_green_bouncer_fx);
		return;
	}


	for (uint i = 0; i < lights.Count(); ++i)
	{
		if (bodyA == lights[i].body)
		{
			if (lights[i].on == false)
			{
				lights[i].on = true;
				App->audio->PlayFx(lights[i].fx);
				switch (lights[i].type)
				{ 
				case green_box:
					counter_box += 1;
					score += 100;
					break;

				case red_box :
					counter_box += 1;
					score += 100;
					break;

				case pink_box:
					counter_box += 1;
					score += 100;
					break;

				case yellow_box:
					counter_box += 1;
					score += 100;
					break;

				case blue_box:
					counter_box += 1;
					score += 100;
					break;

				case orange_box:
					counter_box += 1;
					score += 100;
					break;

				case boy_light:
					counter_char_box += 1;
					score += 200;
					break;

				case girl_light:
					counter_char_box += 1;
					score += 200;
					break;

				case yellow_light:
					counter_yellow_lights += 1;
					score += 100;
					break;

				case green_rectangle:
					counter_green_rectangles += 1;
					score += 100;
					break;
				}


			}
			return;
		}
	}

	if (player_lose == bodyA)
	{
		App->player->ball_start->SetLinearVelocity(0, 0);
		App->player->ball_start->SetAngularVelocity(0);
		App->player->ball_start->SetPosition(516, 823);
		lives--;

		App->audio->PlayFx(player_lose_fx);
		
		if (lives <= 0)
		{
			last_score = score;
			score = 0;
			lives = 3;

			for (uint i = 0; i < lights.Count(); ++i)
			{
				if (lights[i].on == true)
				{
					lights[i].on = false;
					counter_box = 0;
					counter_char_box = 0;
					counter_yellow_lights = 0;
					counter_green_rectangles = 0;


				}
			}
			
		}
		return;
	}



}

Light::Light(ModuleSceneIntro* scene, int x, int y, lightTypes type)
{
	this->type = type;
	this->x = x;
	this->y = y;
	
	int radius;
	int width = 68;
	int height = 59;

	switch (type)
	{
		case green_box:
			texture = scene->green_cube_texture;
			fx = scene->color_box_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case red_box:
			texture = scene->red_cube_texture;
			fx = scene->color_box_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case pink_box:
			texture = scene->pink_cube_texture;
			fx = scene->color_box_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case yellow_box:
			texture = scene->yellow_cube_texture;
			fx = scene->color_box_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case blue_box:
			texture = scene->blue_cube_texture;
			fx = scene->color_box_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case orange_box:
			texture = scene->orange_cube_texture;
			fx = scene->color_box_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+25, width, height);
			body->listener = scene;
			break;

		case girl_light:
			texture = scene->girl_texture;
			fx = scene->char_touch_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case boy_light:
			texture = scene->boy_texture;
			fx = scene->char_touch_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

		case yellow_light:
			radius = 20;
			texture = scene->yellow_light_texture;
			fx = scene->yellow_light_fx;
			body = scene->App->physics->CreateCircleStatic(x+30, y+30, radius, 1.0f, 0.0f, false, true);
			body->listener = scene;
			break;

		case green_rectangle:
			texture = scene->green_rectangle_texture;
			fx = scene->green_rectangle_fx;
			body = scene->App->physics->CreateRectangleSensor(x+33, y+33, width, height);
			body->listener = scene;
			break;

	}
	on = false;
}
