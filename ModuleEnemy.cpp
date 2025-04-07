#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
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
	idle.speed = 8.0f;
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
	backward.speed = 10.0f;
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
	forward.speed = 10.0f;
	forward.loop = true;
	animator.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 11, 436, 97, 94 });
	light_punch.frames.push_back({ 129, 433, 178, 97 });
	light_punch.frames.push_back({ 348, 433, 120, 97 });
	light_punch.speed = 8.0f;
	light_punch.loop = false;
	animator.AddAnimation("light_punch", light_punch);

	// Light kick punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 7, 730, 84, 97 });
	medium_punch.frames.push_back({ 97, 732, 116, 95 });
	medium_punch.frames.push_back({ 220, 733, 198, 95 });
	medium_punch.speed = 8.0f;
	medium_punch.loop = false;
	animator.AddAnimation("medium_punch", medium_punch);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 1243, 1913, 94, 113 });
	victory.frames.push_back({ 1344, 1896, 116, 130 });
	victory.frames.push_back({ 1489, 1875, 100, 149 });
	victory.speed = 10.0f;
	victory.loop = false;
	animator.AddAnimation("victory", victory);

	// Get hurt animation
	Animation hurt;
	hurt.frames.push_back({ 448, 1631, 96, 85 });
	hurt.frames.push_back({ 549, 1637, 100, 80 });
	hurt.frames.push_back({ 655, 1644, 106, 72 });
	hurt.frames.push_back({ 767, 1625, 96, 90 });
	hurt.speed = 10.0f;
	hurt.loop = false;
	animator.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 448, 1631, 96, 85 });
	die.frames.push_back({ 615, 1928, 118, 85 });
	die.frames.push_back({ 742, 1930, 127, 85 });
	die.frames.push_back({ 881, 1973, 161, 52 });
	die.frames.push_back({ 1049, 1992, 177, 39 });
	die.speed = 8.0f;
	die.loop = false;
	animator.AddAnimation("die", die);

	// Start animation
	Animation start;
	start.frames.push_back({ 9, 2108, 85, 63 });
	start.frames.push_back({ 104, 2099, 86, 72 });
	start.frames.push_back({ 201, 2084, 93, 87 });
	start.frames.push_back({ 104, 2099, 86, 72 });
	start.frames.push_back({ 201, 2084, 93, 87 });
	start.frames.push_back({ 515, 2050, 71, 121 });
	start.frames.push_back({ 597, 2049, 93, 122 });
	start.frames.push_back({ 696, 2052, 88, 119 });
	start.frames.push_back({ 793, 2052, 76, 119 });
	start.frames.push_back({ 884, 2059, 67, 112 });
	start.frames.push_back({ 965, 2057, 93, 114 });
	start.frames.push_back({ 1069, 2090, 104, 83 });
	start.frames.push_back({ 201, 2084, 93, 87 });
	start.frames.push_back({ 104, 2099, 86, 72 });
	start.frames.push_back({ 9, 2108, 85, 63 });
	start.frames.push_back({ 104, 2099, 86, 72 });
	start.frames.push_back({ 201, 2084, 93, 87 });
	start.loop = false;
	start.speed = 10.0f;
	animator.AddAnimation("start", start);

	// Set default animation
	animator.SetDefaultAnimation("idle");

	// Set the hitbox parameters
	hitbox.parent = this;
}

ModuleEnemy::~ModuleEnemy()
{
}

// Load assets
bool ModuleEnemy::Start()
{
	LOG("Loading enemy");

	// Set the position
	position.x = 300;
	position.y = 215;

	timer = 0;

	graphics = App->textures->Load("blanka.png"); // arcade version

	return ModuleCharacter::Start();
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
	if (state == DEAD) return;

	ModuleCharacter::Move();

	if (App->player->position.x > position.x)
	{
		is_flipped = false;
	}
	else
	{
		is_flipped = true;
	}

	hitbox.area = { position.x - 25, position.y - 85, 50, 80 };
}

void ModuleEnemy::DrawToScreen()
{
	SDL_Rect currentFrame;

	switch (state)
	{
	case START:
		currentFrame = animator.AnimateAction("start");
		if (animator.AnimationFinished()) state = IDLE;
		break;
	case IDLE:
		currentFrame = animator.AnimateAction("idle");
		break;
	case MOVEMENT:
		if (speed > 0.0f && !is_flipped || speed < 0.0 && is_flipped)
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
			if (animator.GetCurrentFrameNum() == 2)
				is_flipped ? Hit({ position.x - 60, position.y - 75 }, 10) : Hit({ position.x + 60, position.y - 75 }, 30);
			if (animator.AnimationFinished()) state = IDLE;
			break;
		case M_PUNCH:
			currentFrame = animator.AnimateAction("medium_punch");
			if (animator.GetCurrentFrameNum() == 2)
				is_flipped ? Hit({ position.x - 80, position.y - 60 }, 10) : Hit({ position.x + 80, position.y - 60 }, 30);
			if (animator.AnimationFinished()) state = IDLE;
			break;
		}
		break;
	case HURT:
		currentFrame = animator.AnimateAction("hurt");
		if (animator.AnimationFinished())
		{
			is_hurt = false;
			state = IDLE;
		}
		break;
	case DEAD:
		currentFrame = animator.AnimateAction("die");
		if (animator.AnimationFinished()) is_alive = false;
		break;
	case VICTORY:
		currentFrame = animator.AnimateAction("victory");
		break;
	default:
		currentFrame = animator.AnimateAction("idle");
		break;
	}

	// Speed of 3 to match the camera speed, don't really know why
	App->renderer->Blit(graphics, position.x - (currentFrame.w / 2), position.y - currentFrame.h, &currentFrame, SCREEN_SIZE, is_flipped);
}

void ModuleEnemy::DoSomething()
{
	if (!is_enabled) return;

	if (state == HURT || state == DEAD || state == VICTORY)
	{
		speed = 0;
		return;
	}

	if (timer > time_to_act)
	{
		timer = 0;

		// Do smoething random, p. ex: walk, crouch, hit
		int random = rand() % 100;
		
		if (random < 20)
		{
			// Walk forward
			state = MOVEMENT;
			speed = 1.0f;
		}
		else if (random < 40)
		{
			// Walk backwards
			state = MOVEMENT;
			speed = -1.0f;
		}
		else if (random < 50)
		{
			// Idle
			//LOG("IDLE");
			speed = 0;
			state = IDLE;
		}
		else if (random < 80)
		{
			//Light punch
			speed = 0;
			state = COMBAT;
			attackState = L_PUNCH;
		}
		else
		{
			//Light kick
			speed = 0;
			state = COMBAT;
			attackState = M_PUNCH;
		}
		
	}

	timer += App->delta;
}

void ModuleEnemy::Hit(const iPoint& position, const int area)
{
	punch_box.area.x = position.x - (area / 2);
	punch_box.area.y = position.y - (area / 2);
	punch_box.area.w = punch_box.area.h = area;

	if (punch_box.IsColliding(App->player->hitbox))
	{
		App->player->TakeDamage(1);
	}
}

void ModuleEnemy::WinMatch()
{
	if (state == VICTORY) return;
	ModuleCharacter::WinMatch();
	App->audio->PlayFx(1, 0);
}
