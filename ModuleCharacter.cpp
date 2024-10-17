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

	if (position.x < 0) position.x = 0;
	if (position.x > positionLimit) position.x = positionLimit;
}

