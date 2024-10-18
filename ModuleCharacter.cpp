#include "Globals.h"
#include "Application.h"
#include "ModuleCharacter.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

ModuleCharacter::ModuleCharacter(bool start_enabled) : Module(start_enabled)
{
	minPositionLimit = 0;
	maxPositionLimit = 500;
	state = IDLE;
	attackState = NO_ATTACK;
	isFlipped = false;
}

ModuleCharacter::~ModuleCharacter()
{}

// Update
update_status ModuleCharacter::Update()
{
	Move();
	return UPDATE_CONTINUE;
}

void ModuleCharacter::Move()
{
	position.x += (int)speed;

	if (position.x < minPositionLimit) position.x = minPositionLimit;
	if (position.x > maxPositionLimit) position.x = maxPositionLimit;
}

void ModuleCharacter::SetPositionLimits(int min, int max)
{
	minPositionLimit = min + 30;
	maxPositionLimit = max - 30;
}

