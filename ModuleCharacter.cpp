#include "Globals.h"
#include "Application.h"
#include "ModuleCharacter.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

ModuleCharacter::ModuleCharacter(bool start_enabled) : Module(start_enabled)
{
	positionLimit = 0;
	state = IDLE;
	attackState = NO_ATTACK;
}

// Update
update_status ModuleCharacter::Update()
{
	Move();
	DrawToScreen();

	return UPDATE_CONTINUE;
}

void ModuleCharacter::Move()
{
	position.x += (int)speed;

	if (position.x < 0) position.x = 0;
	if (position.x > positionLimit) position.x = positionLimit;
}

void ModuleCharacter::DrawToScreen()
{
	SDL_Rect currentFrame;
	bool finished = false;

	switch (state)
	{
		case IDLE:
			currentFrame = idle.GetCurrentFrame();
			break;
		case MOVEMENT:
			if (speed > 0.0f)
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
	App->renderer->Blit(graphics, position.x, position.y - currentFrame.h, &currentFrame, SCREEN_SIZE);
}

