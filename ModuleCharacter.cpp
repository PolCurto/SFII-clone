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
	is_flipped = false;
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
	if (state == DEAD || state == VICTORY) return;

	position.x += speed;

	if (position.x < minPositionLimit) position.x = minPositionLimit;
	if (position.x > maxPositionLimit) position.x = maxPositionLimit;
}

void ModuleCharacter::SetPositionLimits(int min, int max)
{
	minPositionLimit = min + 30;
	maxPositionLimit = max - 30;
}

void ModuleCharacter::TakeDamage(int damage)
{
	if (is_hurt) return;

	is_hurt = true;
	life -= damage;
	if (life > 0) state = HURT;
	else state = DEAD;
}

void ModuleCharacter::WinMatch()
{
	state = VICTORY;
}

void ModuleCharacter::StartMatch()
{
	state = START;
	is_enabled = true;
}

