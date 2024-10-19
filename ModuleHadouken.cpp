#include "Globals.h"
#include "Application.h"
#include "ModuleHadouken.h"
#include "ModulePlayer.h"
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
}

ModuleHadouken::~ModuleHadouken()
{}

bool ModuleHadouken::Start()
{
	LOG("Hadouken!");
	timer = 0.0f;
	finished = false;

	if (isFlipped) position.x = App->player->position.x - 70;
	else position.x = App->player->position.x + 70;

	position.y = App->player->position.y - 75;

	graphics = App->textures->Load("ryu4.png");
	despawn.Reset();

	return true;
}

bool ModuleHadouken::CleanUp()
{
	LOG("Hadouken out");

	App->textures->Unload(graphics);
	return true;
}

update_status ModuleHadouken::Update()
{
	if (timer < timeToKill)
	{
		//LOG("Timer: %f", timer);
		Move();
		App->renderer->Blit(graphics, position.x, position.y, &sprite, SCREEN_SIZE, isFlipped);
		timer += 0.02f;
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
	App->renderer->Blit(graphics, position.x, position.y, &(despawn.GetCurrentFrame()), SCREEN_SIZE, isFlipped);
	finished = despawn.finished;
}

void ModuleHadouken::SetPosition(int x, int y)
{
	LOG("SET POS");
	position.x = x;
	position.y = y;
}