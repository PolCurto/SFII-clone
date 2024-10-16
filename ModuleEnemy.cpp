#include "Globals.h"
#include "Application.h"
#include "ModuleEnemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModuleEnemy::ModuleEnemy(bool start_enabled) : Module(start_enabled)
{

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({ 9, 13, 97, 95 });
	idle.frames.push_back({ 115, 12, 96, 96 });
	idle.frames.push_back({ 218, 10, 95, 98 });
	idle.frames.push_back({ 115, 12, 96, 96 });
	idle.speed = 0.05f;

	// walk backward animation (arcade sprite sheet)


	// walk forward animation


	// Light punch animation


	// Medium punch animation


	// Hadouken animation
}

ModuleEnemy::~ModuleEnemy()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading enemy");

	// Set the position
	position.x = 200;
	position.y = 216;

	graphics = App->textures->Load("blanka.png"); // arcade version

	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading enemy");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModuleEnemy::Update()
{
	// Update player position before drawing
	Move();

	DrawEnemy();

	return UPDATE_CONTINUE;
}

void ModuleEnemy::Move()
{
	/*
	position.x += (int)speed;

	if (position.x < 0) position.x = 0;
	if (position.x > positionLimit) position.x = positionLimit;
	*/
}

void ModuleEnemy::DrawEnemy()
{
	iPoint enemyPosition = App->enemy->position;

	SDL_Rect currentFrame;
	bool finished = false;

	switch (state)
	{
	case EIDLE:
		currentFrame = idle.GetCurrentFrame();
		break;
	case EMOVEMENT:
		if (speed > 0.0f)
		{
			currentFrame = forward.GetCurrentFrame();
		}
		else
		{
			currentFrame = backward.GetCurrentFrame();
		}
		break;
	case ECOMBAT:
		switch (attackState)
		{
		case EL_PUNCH:
			currentFrame = light_punch.GetCurrentFrameLimited(finished);
			break;
		case EM_PUNCH:
			currentFrame = medium_punch.GetCurrentFrameLimited(finished);
			break;
		}
		break;
	}

	if (finished) state = EIDLE;

	// Speed of 3 to match the camera speed, don't really know why
	App->renderer->Blit(graphics, enemyPosition.x, enemyPosition.y - currentFrame.h, &currentFrame, SCREEN_SIZE, true);

	LOG("Enemy update");
}

