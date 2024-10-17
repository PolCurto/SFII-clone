#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModuleEnemy::ModuleEnemy(bool start_enabled) : ModuleCharacter(start_enabled)
{

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({ 9, 13, 97, 95 });
	idle.frames.push_back({ 115, 12, 96, 96 });
	idle.frames.push_back({ 218, 10, 95, 98 });
	idle.frames.push_back({ 115, 12, 96, 96 });
	idle.speed = 0.04f;

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
	position.x = 300;
	position.y = 215;

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
	ModuleCharacter::Update();
	DrawToScreen();
	return UPDATE_CONTINUE;
}

void ModuleEnemy::Move()
{
	ModuleCharacter::Move();
	if (App->player->position.x > position.x)
	{
		isFlipped = false;
	}
	else
	{
		isFlipped = true;
	}
}

void ModuleEnemy::DrawToScreen()
{
	SDL_Rect currentFrame;
	bool finished = false;

	switch (state)
	{
	case IDLE:
		currentFrame = idle.GetCurrentFrame();
		break;
	case MOVEMENT:
		if (speed > 0.0f && !isFlipped || speed < 0.0 && isFlipped)
		{
			currentFrame = forward.GetCurrentFrame();
		}
		else
		{
			currentFrame = backward.GetCurrentFrame();
		}
		break;
	case COMBAT:
		switch (attackState)
		{
		case L_PUNCH:
			currentFrame = light_punch.GetCurrentFrameLimited(finished);
			break;
		case M_PUNCH:
			currentFrame = medium_punch.GetCurrentFrameLimited(finished);
			break;
		}
		break;
	default:
		currentFrame = idle.GetCurrentFrame();
		break;
	}

	if (finished) state = IDLE;

	// Speed of 3 to match the camera speed, don't really know why
	App->renderer->Blit(graphics, position.x, position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, isFlipped);
}
