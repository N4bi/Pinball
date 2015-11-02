#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	//Graphics
	ball = App->textures->Load("Game/pinball/ball.png");
	flipperDL_texture = App->textures->Load("Game/pinball/flipper.png");
	flipperDR_texture = App->textures->Load("Game/pinball/flipper_right.png");
	flipperUL_texture = App->textures->Load("Game/pinball/flipperUp_left.png");
	flipperUR_texture = App->textures->Load("Game/pinball/flipperUp_right.png");
	spring_texture = App->textures->Load("Game/pinball/spring.png");

	//SFX
	flipper_fx = App->audio->LoadFx("Game/pinball/sounds/flipper.wav");
	spring_down_fx = App->audio->LoadFx("Game/pinball/sounds/spring_down.wav");
	spring_up_fx = App->audio->LoadFx("Game/pinball/sounds/spring_up.wav");
	
	//Flipper vertex

	int flipper_down_left[16] = {

		9, 2,
		22, 3,
		61, 43,
		61, 53,
		49, 53,
		3, 27,
		0, 14,
		6, 4
	};

	int flipper_down_right[16] = {

		2, 43,
		41, 2,
		56, 2,
		62, 12,
		60, 25,
		10, 54,
		2, 52,
		0, 47
	};

	int flipperUp_left[16] = {
		8, 1,
		15, 1,
		44, 31,
		44, 40,
		35, 40,
		2, 20,
		1, 11,
		6, 2
	};

	int flipperUp_right[16] = {
		38, 1,
		30, 1,
		1, 31,
		3, 39,
		10, 41,
		45, 20,
		45, 8,
		40, 3
	};

	//Flippers & Spring & Ball
	flipperDL = App->physics->CreateFlipper(177, 953, 177 + 16, 953 + 16, flipper_down_left, 16, 16, 16, 0, 0, 0.0f, 80.0f, 1.0f, 0.0f, false, false, flipperDL_texture);
	flipperDR = App->physics->CreateFlipper(280, 953, 350 - 16, 953 + 16, flipper_down_right, 16, 46, 16, 0, 0, -80.0f, 0.0f, 1.0f, 0.0f, false, false, flipperDR_texture);
	flipperUL = App->physics->CreateFlipper(355, 213, 355 + 12, 213 + 16, flipperUp_left, 16, 16, 16, 0, 0, 0.0f, 60.0f, 1.0f, 0.0f, false, false, flipperUL_texture);
	flipperUR = App->physics->CreateFlipper(475, 213, 475 - 10, 213 + 16, flipperUp_right, 16, 30, 16, 0, 0, -60.0f, 0.0f, 1.0f, 0.0f, false, false, flipperUR_texture);
	flipperML = App->physics->CreateFlipper(115, 469, 115, 469 + 10, flipperUp_left, 16, 10, 10, 0, 0, -28.0f, 30.0f, 1.0f, 0.0f, false, false, flipperUL_texture);
	flipperMR = App->physics->CreateFlipper(489, 589, 489, 589 + 10, flipperUp_right, 16, 37, 13, 0, 0, -40.0f, 15.0f, 1.0f, 0.0f, false, false, flipperUR_texture);
	spring = App->physics->AddSpring(515, 980, spring_texture);
	ball_start = App->physics->CreateCircle(516, 823, 12);
	ball_start->listener = this;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(ball);
	App->textures->Unload(spring_texture);
	App->textures->Unload(flipperDL_texture);
	App->textures->Unload(flipperDR_texture);
	App->textures->Unload(flipperUL_texture);
	App->textures->Unload(flipperUR_texture);

	App->physics->DestroyBody(ball_start);
	App->physics->DestroyBody(spring);
	App->physics->DestroyBody(flipperDL);
	App->physics->DestroyBody(flipperDR);
	App->physics->DestroyBody(flipperUL);
	App->physics->DestroyBody(flipperUR);
	App->physics->DestroyBody(flipperML);
	App->physics->DestroyBody(flipperMR);


	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	float spring_push = 0.0f;

	//Draw stuff
	int x, y;
	flipperDL->GetPosition(x, y);
	App->renderer->Blit(flipperDL_texture, x, y, NULL, 1.0f, flipperDL->GetAngle(), 0, 0);

	flipperDR->GetPosition(x, y);
	App->renderer->Blit(flipperDR_texture, x, y, NULL, 1.0f, flipperDR->GetAngle(), 0, 0);

	flipperUL->GetPosition(x, y);
	App->renderer->Blit(flipperUL_texture, x, y, NULL, 1.0f, flipperUL->GetAngle(), 0, 0);

	flipperUR->GetPosition(x, y);
	App->renderer->Blit(flipperUR_texture, x, y, NULL, 1.0f, flipperUR->GetAngle(), 0, 0);

	flipperML->GetPosition(x, y);
	App->renderer->Blit(flipperUL_texture, x, y, NULL, 1.0f, flipperML->GetAngle(), 0, 0);

	flipperMR->GetPosition(x, y);
	App->renderer->Blit(flipperUR_texture, x, y, NULL, 1.0f, flipperMR->GetAngle(), 0, 0);

	spring->GetPosition(x, y);
	App->renderer->Blit(spring_texture, x, y);

	ball_start->GetPosition(x, y);
	App->renderer->Blit(ball, x, y);

	p2List_item<PhysBody*>* c = balls.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	///////////////////////////////////////////////////////////////

	//Key stuff

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			App->audio->PlayFx(flipper_fx);
		}
		flipperDL->Turn(-360);
		flipperUL->Turn(-360);
		flipperML->Turn(-360);

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		flipperDL->Turn(360);
		flipperUL->Turn(360);
		flipperML->Turn(360);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			App->audio->PlayFx(flipper_fx);
		}

		flipperDR->Turn(360);
		flipperUR->Turn(360);
		flipperMR->Turn(360);

	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		flipperDR->Turn(-360);
		flipperUR->Turn(-360);
		flipperMR->Turn(-360);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			App->audio->PlayFx(spring_down_fx);
		}

		spring_push += 175.0f / 5.0f;
		spring->Push(0, spring_push);

	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		App->audio->PlayFx(spring_up_fx);
		spring_push = 0.0f;
	}

	//Key stuff - DEBUG

	if (App->physics->GetDebug() == true)
	{

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			balls.add(App->physics->CreateCircle(516, 823, 12, false, false));
			balls.getLast()->data->listener = this;
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12, false, false));
			balls.getLast()->data->listener = this;
		}
	}
	return UPDATE_CONTINUE;
}



