#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = ball = table = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("Game/pinball/wheel.png"); 
	box = App->textures->Load("Game/pinball/crate.png");
	rick = App->textures->Load("Game/pinball/rick_head.png");
	ball = App->textures->Load("Game/pinball/ball.png");
	table = App->textures->Load("Game/pinball/ground3.png");
	flipper_texture = App->textures->Load("Game/pinball/flipper.png");
	spring_texture = App->textures->Load("Game/pinball/spring.png");
	

	bonus_fx = App->audio->LoadFx("Game/pinball/bonus.wav");

	
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	

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

	int launcher[90] = {
		351, 1022,
		371, 1006,
		488, 944,
		490, 766,
		489, 753,
		482, 733,
		470, 712,
		456, 698,
		440, 690,
		440, 685,
		445, 676,
		461, 684,
		467, 691,
		478, 702,
		487, 715,
		494, 730,
		500, 747,
		501, 752,
		502, 765,
		502, 1022,
		530, 1022,
		530, 757,
		528, 742,
		526, 733,
		523, 725,
		520, 718,
		514, 705,
		507, 695,
		500, 684,
		489, 673,
		479, 663,
		467, 657,
		455, 652,
		462, 637,
		467, 635,
		478, 641,
		477, 647,
		495, 661,
		515, 683,
		534, 717,
		541, 743,
		543, 774,
		543, 860,
		542, 1024,
		352, 1024
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

	int flipper[16] = {
	/*	36, 46,
		48, 53,
		52, 55,
		57, 55,
		61, 51,
		62, 46,
		61, 41,
		53, 33,
		21, 3,
		14, 1,
		8, 3,
		4, 8,
		1, 14,
		2, 22,
		3, 27,
		10, 31,
		34, 45*/

	/*	186, 950,
		195, 950,
		201, 953,
		213, 965,
		238, 989,
		239, 995,
		238, 1002,
		232, 1005,
		222, 1000,
		182, 978,
		178, 968,
		179, 959,
		184, 952*/

		9, 2,
		22, 3,
		61, 43,
		61, 53,
		49, 53,
		3, 27,
		0, 14,
		6, 4
	};

	int flipper_wheel1[16] = {

		191, 958,
		186, 959,
		183, 965,
		187, 972,
		194, 972,
		199, 966,
		198, 960,
		194, 958
	};

	walls.add(App->physics->AddWall(0, 0, table, 70));
	walls.add(App->physics->AddWall(0, 0, triangle_right, 44));
	walls.add(App->physics->AddWall(0, 0, launcher, 90));
	
	walls.add(App->physics->AddWall(0, 0, triangle_left, 38));
	walls.add(App->physics->AddWall(0, 0, triangle_left2, 12));

	flipper1 = App->physics->AddFlipper(flipper_texture);
	spring = App->physics->AddSpring(515, 980, spring_texture);


	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{ 
	float spring_push = 0.0f;

	App->renderer->Blit(table, 0, 0);

	int x, y;
	flipper1->GetPosition(x, y);
	App->renderer->Blit(flipper_texture, x, y);

	spring->GetPosition(x, y);
	App->renderer->Blit(spring_texture, x, y);



	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}


	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
		balls.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->physics->m_joint->SetMotorSpeed(1000.0f);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		App->physics->m_joint->SetMotorSpeed(-1000.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN == KEY_DOWN) || App->input->GetKey(SDL_SCANCODE_DOWN == KEY_REPEAT))
	{
		App->physics->spring_joint->SetMotorSpeed(-20.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN == KEY_UP))
	{
		App->physics->spring_joint->SetMotorSpeed(20.0f);
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = balls.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	

	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}
