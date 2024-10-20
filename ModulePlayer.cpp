#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : ModuleCharacter(start_enabled)
{
	// idle animation (arcade sprite sheet)
	Animation idle;
	idle.frames.push_back({7, 14, 60, 90});
	idle.frames.push_back({95, 15, 60, 89});
	idle.frames.push_back({184, 14, 60, 90});
	idle.frames.push_back({276, 11, 60, 93});
	idle.frames.push_back({366, 12, 60, 92});
	idle.speed = 10.0f;
	idle.loop = true;
	animator.AddAnimation("idle", idle);
	
	// walk backward animation (arcade sprite sheet)
	Animation backward;
	backward.frames.push_back({542, 131, 61, 87});
	backward.frames.push_back({628, 129, 59, 90});
	backward.frames.push_back({713, 128, 57, 90});
	backward.frames.push_back({797, 127, 57, 90});
	backward.frames.push_back({883, 128, 58, 91});
	backward.frames.push_back({974, 129, 57, 89});
	backward.speed = 12.0f;
	backward.loop = true;
	animator.AddAnimation("walk_b", backward);

	// TODO 8: setup the walk forward animation from ryu4.png -- Done
	Animation forward;
	forward.frames.push_back({ 9, 136, 53, 83 });
	forward.frames.push_back({ 78, 131, 60, 88 });
	forward.frames.push_back({ 162, 128, 64, 92 });
	forward.frames.push_back({ 259, 128, 63, 90 });
	forward.frames.push_back({ 352, 128, 54, 91 });
	forward.frames.push_back({ 432, 131, 50, 89 });
	forward.speed = 12.0f;
	forward.loop = true;
	animator.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 13, 272, 70, 91 });
	light_punch.frames.push_back({ 98, 272, 126, 91 });
	light_punch.frames.push_back({ 13, 272, 70, 91 });
	light_punch.speed = 10.0f;
	light_punch.loop = false;
	animator.AddAnimation("light_punch", light_punch);

	// Medium punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 283, 269, 138, 94 });
	medium_punch.frames.push_back({ 453, 268, 138, 93 });
	medium_punch.frames.push_back({ 611, 268, 138, 92 });
	medium_punch.frames.push_back({ 453, 268, 138, 93 });
	medium_punch.frames.push_back({ 283, 269, 138, 94 });
	medium_punch.speed = 8.0f;
	medium_punch.loop = false;
	animator.AddAnimation("medium_punch", medium_punch);

	// Hadouken animation
	Animation hadouken_anim;
	hadouken_anim.frames.push_back({ 18, 1545, 90, 90 });
	hadouken_anim.frames.push_back({ 176, 1551, 116, 86 });
	hadouken_anim.frames.push_back({ 379, 1552, 102, 87 });
	hadouken_anim.frames.push_back({ 609, 1558, 118, 77 });
	hadouken_anim.frames.push_back({ 609, 1558, 118, 77 });
	hadouken_anim.speed = 10.0f;
	hadouken_anim.loop = false;
	animator.AddAnimation("hadouken", hadouken_anim);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 500, 2479, 42, 83 });
	victory.frames.push_back({ 577, 2474, 56, 88 });
	victory.frames.push_back({ 660, 2465, 60, 97 });
	victory.frames.push_back({ 745, 2440, 55, 122 });
	victory.speed = 6.0f;
	victory.loop = false;
	animator.AddAnimation("victory", victory);

	// Hurt animation
	Animation hurt;
	hurt.frames.push_back({ 398, 2094, 58, 85 });
	hurt.frames.push_back({ 482, 2097, 66, 82 });
	hurt.frames.push_back({ 30, 2101, 68, 79 });
	hurt.frames.push_back({ 117, 2090, 62, 90 });
	hurt.speed = 10.0f;
	hurt.loop = false;
	animator.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 350, 2233, 124, 63 });
	die.frames.push_back({ 687, 2247, 119, 44 });
	die.frames.push_back({ 849, 2246, 123, 41 });
	die.frames.push_back({ 984, 2265, 128, 31 });
	die.speed = 6.0f;
	die.loop = false;
	animator.AddAnimation("die", die);

	// Start animation
	Animation start;
	start.frames.push_back({ 47, 2467, 52, 97 });
	start.frames.push_back({ 124, 2469, 54, 95 });
	start.frames.push_back({ 206, 2469, 54, 95 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.frames.push_back({ 289, 2469, 54, 95 });
	start.frames.push_back({ 380, 2468, 54, 96 });
	start.speed = 10.0f;
	start.loop = false;
	animator.AddAnimation("start", start);

	// Set the starting animation;
	animator.SetDefaultAnimation("idle");

	hitbox.parent = this;

	debugRect = { 1061, 2457, 20, 20 };

}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// Set the position
	position.x = 100;
	position.y = 215;

	graphics = App->textures->Load("ryu4.png"); // arcade version

	hadouken = App->hadouken;

	return ModuleCharacter::Start();
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	hadouken = nullptr;

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// Update player position before drawing
	CheckPlayerInputs();
	ModuleCharacter::Update();
	DrawToScreen();

	if (hadouken->IsEnabled() && (hadouken->finished))
	{
		hadouken->Disable();
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::Move()
{
	if (state == DEAD || state == VICTORY) return;

	ModuleCharacter::Move();

	if (App->enemy->position.x > position.x)
	{
		is_flipped = false;
	}
	else
	{
		is_flipped = true;
	}

	hitbox.area = { position.x - 20, position.y - 85, 40, 80 };

	//LOG("Hitbox x1: %d, x2: %d, y1: %d, y2: %d", hitbox.area.x, hitbox.area.x + hitbox.area.w, hitbox.area.y, hitbox.area.y + hitbox.area.h);
}

void ModulePlayer::CheckPlayerInputs()
{
	if (!is_enabled || state == DEAD || state == VICTORY || state == START) return;

	if (state != COMBAT)
	{
		// Right movement
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			speed = 1.0f;
			state = MOVEMENT;
		}
		// Left Movement
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			speed = -1.0f;
			state = MOVEMENT;
		}
		else
		{
			speed = 0.0f;
			state = IDLE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		speed = 0.0f;
		state = COMBAT;
		attackState = L_PUNCH;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		speed = 0.0f;
		state = COMBAT;
		attackState = M_PUNCH;
	}
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN && !hadouken->IsEnabled())
	{
		speed = 0.0f;
		state = COMBAT;
		attackState = HADOUKEN;
	}
}

void ModulePlayer::DrawToScreen()
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
				if (animator.GetCurrentFrameNum() == 1)
				{
					is_flipped ? Hit({ position.x - 60, position.y - 70 }, 10) : Hit({ position.x + 60, position.y - 70 }, 10);
				}
				if (animator.AnimationFinished()) state = IDLE;
				break;
			case M_PUNCH:
				currentFrame = animator.AnimateAction("medium_punch");
				if (animator.AnimationFinished()) state = IDLE;
				break;
			case HADOUKEN:
				currentFrame = animator.AnimateAction("hadouken");
				if (animator.GetCurrentFrameNum() == 3) ThrowHadouken();
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

	// Debug hitbox
	//App->renderer->Blit(graphics, position.x - 20, position.y - 85, &hitbox.area, SCREEN_SIZE);
}

void ModulePlayer::Hit(iPoint position, int area)
{
	punch_box.area.x = position.x - (area / 2);
	punch_box.area.y = position.y - (area / 2);
	punch_box.area.w = punch_box.area.h = area;

	debugRect.w = debugRect.h = area;

	if (punch_box.IsColliding(App->enemy->hitbox))
	{
		App->enemy->TakeDamage(1);
	}

	App->renderer->Blit(graphics, punch_box.area.x, punch_box.area.y, &debugRect, SCREEN_SIZE);
}

void ModulePlayer::ThrowHadouken()
{
	hadouken->isFlipped = is_flipped;
	hadouken->Enable();
}