#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModuleEnemy::ModuleEnemy(bool start_enabled) : ModuleCharacter(start_enabled)
{

	// idle animation (arcade sprite sheet)
	Animation idle;
	idle.frames.push_back({ 9, 13, 97, 95 });
	idle.frames.push_back({ 115, 12, 96, 96 });
	idle.frames.push_back({ 218, 10, 95, 98 });
	idle.frames.push_back({ 115, 12, 96, 96 });
	idle.speed = 0.08f;
	idle.loop = true;

	animator.AddAnimation("idle", idle);

	// walk backward animation (arcade sprite sheet)
	Animation backward;
	backward.frames.push_back({ 5, 124, 96, 90 });
	backward.frames.push_back({ 118, 128, 78, 90 });
	backward.frames.push_back({ 215, 132, 78, 76 });
	backward.frames.push_back({ 314, 125, 79, 93 });
	backward.frames.push_back({ 413, 124, 84, 94 });
	backward.frames.push_back({ 518, 125, 79, 93 });
	backward.frames.push_back({ 620, 132, 78, 81 });
	backward.frames.push_back({ 720, 128, 72, 86 });
	backward.speed = 0.1f;
	backward.loop = true;

	animator.AddAnimation("walk_b", backward);

	// walk forward animation
	Animation forward;
	forward.frames.push_back({ 335, 31, 82, 72 });
	forward.frames.push_back({ 440, 34, 78, 68 });
	forward.frames.push_back({ 542, 30, 77, 77 });
	forward.frames.push_back({ 639, 25, 86, 82 });
	forward.frames.push_back({ 751, 30, 77, 77 });
	forward.frames.push_back({ 852, 34, 79, 73 });
	forward.frames.push_back({ 945, 31, 85, 76 });
	forward.frames.push_back({ 1033, 26, 99, 77 });
	forward.speed = 0.1f;
	forward.loop = true;

	animator.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 11, 436, 97, 94 });
	light_punch.frames.push_back({ 121, 433, 135, 97 });
	light_punch.frames.push_back({ 268, 433, 109, 97 });
	light_punch.speed = 0.08f;
	light_punch.loop = false;

	animator.AddAnimation("light_punch", light_punch);

	// Light kick punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 10, 730, 81, 97 });
	medium_punch.frames.push_back({ 105, 732, 97, 95 });
	medium_punch.frames.push_back({ 219, 733, 137, 95 });
	medium_punch.speed = 0.08f;
	medium_punch.loop = false;

	animator.AddAnimation("medium_punch", medium_punch);

	// Set default animation
	animator.SetDefaultAnimation("idle");
}

ModuleEnemy::~ModuleEnemy()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading enemy");

	// Set the position
	position.x = 300;
	position.y = 215;

	graphics = App->textures->Load("blanka.png"); // arcade version

	return true;
}

// Unload assets
bool ModuleEnemy::CleanUp()
{
	LOG("Unloading enemy");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModuleEnemy::Update()
{
	DoSomething();
	ModuleCharacter::Update();
	DrawToScreen();
	return UPDATE_CONTINUE;
}

void ModuleEnemy::Move()
{
	ModuleCharacter::Move();
	if (App->player->position.x > position.x)
	{
		isFlipped = false;
	}
	else
	{
		isFlipped = true;
	}
}

void ModuleEnemy::DrawToScreen()
{
	SDL_Rect currentFrame;

	switch (state)
	{
	case IDLE:
		currentFrame = animator.AnimateAction("idle");
		break;
	case MOVEMENT:
		if (speed > 0.0f && !isFlipped || speed < 0.0 && isFlipped)
		{
			currentFrame = animator.AnimateAction("walk_f");
		}
		else
		{
			currentFrame = animator.AnimateAction("walk_b");
		}
		break;
	case COMBAT:
		switch (attackState)
		{
		case L_PUNCH:
			currentFrame = animator.AnimateAction("light_punch");
			if (animator.AnimationFinished()) state = IDLE;
			break;
		case M_PUNCH:
			currentFrame = animator.AnimateAction("medium_punch");
			if (animator.AnimationFinished()) state = IDLE;
			break;
		}
		break;
	default:
		currentFrame = animator.AnimateAction("idle");
		break;
	}

	// Speed of 3 to match the camera speed, don't really know why
	App->renderer->Blit(graphics, position.x - (currentFrame.w / 2), position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, isFlipped);
}

void ModuleEnemy::DoSomething()
{
	if (timer > time_to_act)
	{
		timer = 0;

		// Do smoething random, p. ex: walk, crouch, hit
		int random = rand() % 100;
		
		if (random < 20)
		{
			// Walk forward
			LOG("WALK FOERWARD");
			state = MOVEMENT;
			speed = 3.0f;
		}
		else if (random < 40)
		{
			LOG("WALK BACK");
			// Walk backwards
			state = MOVEMENT;
			speed = -3.0f;
		}
		else if (random < 60)
		{
			// Idle
			LOG("IDLE");
			speed = 0;
			state = IDLE;
		}
		else if (random < 80)
		{
			//Light punch
			LOG("LIGHT PUNCH");
			speed = 0;
			state = COMBAT;
			attackState = L_PUNCH;
		}
		else
		{
			//Light kick
			LOG("LIGHT KICK");
			speed = 0;
			state = COMBAT;
			attackState = M_PUNCH;
		}
		
	}

	timer += 0.02f;
}
