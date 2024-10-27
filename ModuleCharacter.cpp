#include "Globals.h"
#include "Application.h"
#include "ModuleCharacter.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

ModuleCharacter::ModuleCharacter(bool start_enabled) : Module(start_enabled)
{
	minPositionLimit = 0;
	maxPositionLimit = 500;
	state = IDLE;
	attackState = NO_ATTACK;
	is_flipped = false;
	is_enabled = false;
	life = 5;
	is_alive = true;
	is_hurt = false;
	speed = 0.0f;
}

ModuleCharacter::~ModuleCharacter()
{}


bool ModuleCharacter::Start()
{
	speed = 0;
	life = 5;
	is_hurt = false;
	is_enabled = false;
	is_alive = true;
	state = IDLE;

	return true;
}

update_status ModuleCharacter::Update()
{
	Move();
	return UPDATE_CONTINUE;
}

void ModuleCharacter::Move()
{
	if (state == DEAD || state == VICTORY) return;

	if (state == HURT) speed = 0;

	position.x += static_cast<int>(speed);

	if (position.x < minPositionLimit) position.x = minPositionLimit;
	if (position.x > maxPositionLimit) position.x = maxPositionLimit;
}

void ModuleCharacter::SetPositionLimits(const int min, const int max)
{
	minPositionLimit = min + 30;
	maxPositionLimit = max - 30;
}

void ModuleCharacter::TakeDamage(const int damage)
{
	if (is_hurt) return;

	LOG("Player take damage");

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

