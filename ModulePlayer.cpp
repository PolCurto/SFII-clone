#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 100;
	position.y = 216;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({7, 14, 60, 90});
	idle.frames.push_back({95, 15, 60, 89});
	idle.frames.push_back({184, 14, 60, 90});
	idle.frames.push_back({276, 11, 60, 93});
	idle.frames.push_back({366, 12, 60, 92});
	idle.speed = 0.05f;
	
	// walk backward animation (arcade sprite sheet)
	backward.frames.push_back({542, 131, 61, 87});
	backward.frames.push_back({628, 129, 59, 90});
	backward.frames.push_back({713, 128, 57, 90});
	backward.frames.push_back({797, 127, 57, 90});
	backward.frames.push_back({883, 128, 58, 91});
	backward.frames.push_back({974, 129, 57, 89});
	backward.speed = 0.05f;

	// TODO 8: setup the walk forward animation from ryu4.png -- Done
	forward.frames.push_back({ 9, 136, 53, 83 });
	forward.frames.push_back({ 78, 131, 60, 88 });
	forward.frames.push_back({ 162, 128, 64, 92 });
	forward.frames.push_back({ 259, 128, 63, 90 });
	forward.frames.push_back({ 352, 128, 54, 91 });
	forward.frames.push_back({ 432, 131, 50, 89 });
	forward.speed = 0.05f;
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("ryu4.png"); // arcade version

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// Update player position before drawing
	CheckPlayerInputs();
	Move();

	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	// position while cycling the animation(check Animation.h)
	DrawPlayer();
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::CheckPlayerInputs()
{
	speed = 0;
	state = IDLE;

	// Right movement
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		speed = 1.0f;
		state = MOVE_FORWARD;
	}

	// Left Movement
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		speed = -1.0f;
		state = MOVE_BACKWARD;
	}
}

void ModulePlayer::Move()
{
	position.x += speed;
}

void ModulePlayer::DrawPlayer()
{
	iPoint playerPosition = App->player->position;

	SDL_Rect currentFrame;

	switch (state)
	{
		case IDLE:
			currentFrame = idle.GetCurrentFrame();
			break;
		case MOVE_FORWARD:
			currentFrame = forward.GetCurrentFrame();
			break;
		case MOVE_BACKWARD:
			currentFrame = backward.GetCurrentFrame();
	}

	App->renderer->Blit(graphics, playerPosition.x, playerPosition.y - currentFrame.h, &currentFrame);
}

