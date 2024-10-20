#include "Globals.h"
#include "Application.h"
#include "ModuleHadouken.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

ModuleHadouken::ModuleHadouken(bool start_enabled) : Module(start_enabled)
{
	sprite.x = 819;
	sprite.y = 1563;
	sprite.w = 43;
	sprite.h = 32;
	
	despawn.frames.push_back({ 1023, 1565, 28, 28 });
	despawn.frames.push_back({ 1079, 1565, 28, 27 });
	despawn.speed = 14.0f;
	despawn.loop = false;

	time_to_kill = 4.0f;
	hitbox.parent = this;
}

ModuleHadouken::~ModuleHadouken()
{}

bool ModuleHadouken::Start()
{
	LOG("Hadouken!");
	timer = 0.0f;
	finished = false;

	SetPosition();
	enemy = App->enemy;

	graphics = App->textures->Load("ryu4.png");
	despawn.Reset();

	return true;
}

bool ModuleHadouken::CleanUp()
{
	LOG("Hadouken out");

	SetPosition();

	App->textures->Unload(graphics);
	return true;
}

update_status ModuleHadouken::Update()
{
	if (timer < time_to_kill)
	{
		Move();
		App->renderer->Blit(graphics, position.x - (sprite.w / 2), position.y - sprite.h, &sprite, SCREEN_SIZE, isFlipped);
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

void ModuleHadouken::Move()
{
	int speed = isFlipped ? -1 : 1;
	position.x += speed;
}

void ModuleHadouken::Despawn()
{
	SDL_Rect currentFrame = despawn.GetCurrentFrame();
	App->renderer->Blit(graphics, position.x - (currentFrame.w / 2), position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, isFlipped);
	finished = despawn.finished;
}

void ModuleHadouken::SetPosition()
{
	if (isFlipped) position.x = App->player->position.x - 50;
	else position.x = App->player->position.x + 50;

	position.y = App->player->position.y - 45;
}

void ModuleHadouken::CheckCollisions()
{
	if (hitbox.IsColliding(enemy->hitbox))
	{
		enemy->TakeDamage(1);
		timer = time_to_kill;
	}
}