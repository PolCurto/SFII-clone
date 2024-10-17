#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : ModuleCharacter(start_enabled)
{
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
	backward.speed = 0.08f;

	// TODO 8: setup the walk forward animation from ryu4.png -- Done
	forward.frames.push_back({ 9, 136, 53, 83 });
	forward.frames.push_back({ 78, 131, 60, 88 });
	forward.frames.push_back({ 162, 128, 64, 92 });
	forward.frames.push_back({ 259, 128, 63, 90 });
	forward.frames.push_back({ 352, 128, 54, 91 });
	forward.frames.push_back({ 432, 131, 50, 89 });
	forward.speed = 0.08f;

	// Light punch animation
	light_punch.frames.push_back({ 19, 272, 64, 91 });
	light_punch.frames.push_back({ 108, 272, 92, 91 });
	light_punch.frames.push_back({ 19, 272, 64, 91 });
	light_punch.speed = 0.05f;

	// Medium punch animation
	medium_punch.frames.push_back({ 253, 269, 60, 94 });
	medium_punch.frames.push_back({ 333, 268, 66, 93 });
	medium_punch.frames.push_back({ 432, 268, 108, 92 });
	medium_punch.frames.push_back({ 333, 268, 66, 93 });
	medium_punch.frames.push_back({ 253, 269, 60, 94 });
	medium_punch.speed = 0.06f;

	// Hadouken animation
	hadouken_anim.frames.push_back({ 34, 1545, 74, 90 });
	hadouken_anim.frames.push_back({ 135, 1551, 85, 86 });
	hadouken_anim.frames.push_back({ 244, 1552, 90, 87 });
	hadouken_anim.frames.push_back({ 357, 1558, 105, 77 });
	hadouken_anim.speed = 0.05f;
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// Set the position
	position.x = 100;
	position.y = 215;

	graphics = App->textures->Load("ryu4.png"); // arcade version

	hadouken = App->hadouken;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	hadouken = nullptr;

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// Update player position before drawing
	CheckPlayerInputs();
	ModuleCharacter::Update();
	DrawToScreen();

	if (hadouken->IsEnabled() && (hadouken->finished))
	{
		hadouken->Disable();
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::Move()
{
	ModuleCharacter::Move();
	if (App->enemy->position.x > position.x)
	{
		isFlipped = false;
	}
	else
	{
		isFlipped = true;
	}
}

void ModulePlayer::CheckPlayerInputs()
{
	if (state != COMBAT)
	{
		// Right movement
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			speed = 1.0f;
			state = MOVEMENT;
		}
		// Left Movement
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			speed = -1.0f;
			state = MOVEMENT;
		}
		else
		{
			speed = 0.0f;
			state = IDLE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		speed = 0.0f;
		state = COMBAT;
		attackState = L_PUNCH;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		speed = 0.0f;
		state = COMBAT;
		attackState = M_PUNCH;
	}
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN && !hadouken->IsEnabled())
	{
		speed = 0.0f;
		state = COMBAT;
		attackState = HADOUKEN;
	}
}

void ModulePlayer::DrawToScreen()
{
	SDL_Rect currentFrame;

	switch (state)
	{
	case IDLE:
		currentFrame = idle.GetCurrentFrame(true);
		break;
	case MOVEMENT:
		if (speed > 0.0f && !isFlipped || speed < 0.0 && isFlipped)
		{
			currentFrame = forward.GetCurrentFrame(true);
		}
		else
		{
			currentFrame = backward.GetCurrentFrame(true);
		}
		break;
	case COMBAT:
		switch (attackState)
		{
			case L_PUNCH:
				currentFrame = light_punch.GetCurrentFrame(false);
				if (light_punch.finished) state = IDLE;
				break;
			case M_PUNCH:
				currentFrame = medium_punch.GetCurrentFrame(false);
				if (medium_punch.finished) state = IDLE;
				break;
			case HADOUKEN:
				int frame_num = 0;
				currentFrame = hadouken_anim.GetCurrentFrameNum(false, frame_num);
				if (frame_num == 3)
				{
					ThrowHadouken();
				}
				if (hadouken_anim.finished) state = IDLE;
				break;
		}
		break;
	default:
		currentFrame = idle.GetCurrentFrame(true);
		break;
	}

	// Speed of 3 to match the camera speed, don't really know why
	App->renderer->Blit(graphics, position.x, position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, isFlipped);
}

void ModulePlayer::ThrowHadouken()
{
	hadouken->isFlipped = isFlipped;
	hadouken->Enable();
}


