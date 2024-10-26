#include "Globals.h"
#include "Application.h"
#include "ModuleProjectile.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModuleProjectile::ModuleProjectile(bool start_enabled) : Module(start_enabled)
{
	time_to_kill = 4.0f;
	hitbox.parent = this;
}

ModuleProjectile::~ModuleProjectile()
{}

bool ModuleProjectile::Start()
{
	LOG("Hadouken!");
	timer = 0.0f;
	is_finished = false;

	SetPosition();
	enemy = App->enemy;

	return true;
}

bool ModuleProjectile::CleanUp()
{
	LOG("Hadouken out");

	SetPosition();

	App->textures->Unload(graphics);
	return true;
}

update_status ModuleProjectile::Update()
{
	if (timer < time_to_kill)
	{
		Move();
		SDL_Rect currentFrame = animator.AnimateAction("move");
		App->renderer->Blit(graphics, position.x - (currentFrame.w / 2), position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, is_flipped);
		hitbox.area = { position.x - 15, position.y - 10, 30, 20 };
		CheckCollisions();
		timer += App->delta;		
	}
	else
	{
		Despawn();
	}
	return UPDATE_CONTINUE;
}

void ModuleProjectile::Move()
{
	int speed = is_flipped ? -1 : 1;
	position.x += speed;
}

void ModuleProjectile::Despawn()
{
	SDL_Rect currentFrame = animator.AnimateAction("despawn");
	App->renderer->Blit(graphics, position.x - (currentFrame.w / 2), position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, is_flipped);
	is_finished = animator.AnimationFinished();
}

void ModuleProjectile::SetPosition()
{
	if (is_flipped) position.x = App->player->position.x - 50;
	else position.x = App->player->position.x + 50;

	position.y = App->player->position.y - 45;
}

void ModuleProjectile::CheckCollisions()
{
	if (hitbox.IsColliding(enemy->hitbox))
	{
		enemy->TakeDamage(1);
		timer = time_to_kill;
	}
}