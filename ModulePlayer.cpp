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

	graphics = App->textures->Load(character_data.texture_name); // arcade version
	projectile = App->projectile;
	projectile->texture_name = character_data.texture_name;
	projectile->animator = character_data.projectile_animator;

	return ModuleCharacter::Start();
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	projectile = nullptr;

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// Update player position before drawing
	CheckPlayerInputs();
	ModuleCharacter::Update();
	DrawToScreen();

	if (projectile->IsEnabled() && (projectile->is_finished))
	{
		projectile->Disable();
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
	if (!is_enabled || state == DEAD || state == VICTORY || state == START || state == HURT) return;

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
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN && !projectile->on_screen)
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
		currentFrame = character_data.animator.AnimateAction("start");
		if (character_data.animator.AnimationFinished()) state = IDLE;
		break;
	case IDLE:
		currentFrame = character_data.animator.AnimateAction("idle");
		break;
	case MOVEMENT:
		if (speed > 0.0f && !is_flipped || speed < 0.0 && is_flipped)
		{
			currentFrame = character_data.animator.AnimateAction("walk_f");
		}
		else
		{
			currentFrame = character_data.animator.AnimateAction("walk_b");
		}
		break;
	case COMBAT:
		switch (attackState)
		{
			case L_PUNCH:
				currentFrame = character_data.animator.AnimateAction("light_punch");
				if (character_data.animator.GetCurrentFrameNum() == 1)
					is_flipped ? Hit({ position.x - 55, position.y - 73 }, 10) : Hit({ position.x + 55, position.y - 73 }, 15);
				if (character_data.animator.AnimationFinished()) state = IDLE;
				break;
			case M_PUNCH:
				currentFrame = character_data.animator.AnimateAction("medium_punch");
				if (character_data.animator.GetCurrentFrameNum() == 2)
					is_flipped ? Hit({ position.x - 65, position.y - 73 }, 10) : Hit({ position.x + 65, position.y - 73 }, 15);
				if (character_data.animator.AnimationFinished()) state = IDLE;
				break;
			case HADOUKEN:
				currentFrame = character_data.animator.AnimateAction("hadouken");
				if (character_data.animator.GetCurrentFrameNum() == 3) ThrowProjectile();
				if (character_data.animator.AnimationFinished()) state = IDLE;
				break;
		}
		break;
	case HURT:
		currentFrame = character_data.animator.AnimateAction("hurt");
		if (character_data.animator.AnimationFinished())
		{
			is_hurt = false;
			state = IDLE;
		}
		break;
	case DEAD:
		currentFrame = character_data.animator.AnimateAction("die");
		if (character_data.animator.AnimationFinished()) is_alive = false;
		break;
	case VICTORY:
		currentFrame = character_data.animator.AnimateAction("victory");
		break;
	default:
		currentFrame = character_data.animator.AnimateAction("idle");
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

void ModulePlayer::ThrowProjectile()
{
	projectile->is_flipped = is_flipped;
	projectile->Spawn();
}