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
	ball = table = NULL;
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
	
	//Graphics
	ball = App->textures->Load("Game/pinball/ball.png");
	table = App->textures->Load("Game/pinball/ground3.png");

	//Audio

			//Music
	App->audio->PlayMusic("Game/pinball/sounds/music_pinball.ogg");



	//Creation of the initial ball
	ball_start = App->physics->CreateCircle(516, 823, 12);


	
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	
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
	walls.add(App->physics->AddWall(0, 0, bouncer_left, 12, 0.4f));
	walls.add(App->physics->AddWall(0, 0, bouncer_right, 12, 0.4f));


	// Other stuff



///////////////////////////////////////////////////////////////
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(ball);
	App->textures->Unload(table);


	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{ 
	//Draw stuff
	
	App->renderer->Blit(table, 0, 0);

	int x, y;

	ball_start->GetPosition(x, y);
	App->renderer->Blit(ball, x, y, NULL, 1.0f,ball_start->GetRotation());

/////////////////////////////////////////////////////////////////

	//Key stuff

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		balls.add(App->physics->CreateCircle(516, 823, 12));
		balls.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
		balls.getLast()->data->listener = this;
	}

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = balls.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

	
}
