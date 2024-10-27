#include "Globals.h"
#include "Application.h"
#include "ModuleSceneGuile.h"
#include "ModuleStageSelector.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneGuile::ModuleSceneGuile(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 0;
	ground.y = 0;
	ground.w = 672;
	ground.h = 224;

	// coordinates x,y,w,h from ken_stage.png -- Done
	background.x = 0;
	background.y = 687;
	background.w = 558;
	background.h = 78;

	// Background / sky
	sky.x = 5;
	sky.y = 267;
	sky.w = 480;
	sky.h = 99;

	// Fence
	fence.x = 80;
	fence.y = 472;
	fence.w = 94;
	fence.h = 100;

	// Crowd animations
	crowd_1.frames.push_back({ 99, 366, 45, 97 });
	crowd_1.frames.push_back({ 165, 366, 45, 97 });
	crowd_1.speed = 0.75f;

	crowd_2.frames.push_back({ 220, 392, 97, 71 });
	crowd_2.frames.push_back({ 323, 392, 97, 71 });
	crowd_2.speed = 1.5f;

	crowd_3.frames.push_back({ 427, 384, 80, 79 });
	crowd_3.frames.push_back({ 512, 384, 80, 79 });
	crowd_3.speed = 1.0f;

	match_started = false;
}

ModuleSceneGuile::~ModuleSceneGuile()
{}

// Load assets
bool ModuleSceneGuile::Start()
{
	LOG("Loading guile scene");

	is_fading = false;
	match_started = false;
	start_timer = 0;
	end_timer = 0;

	graphics = App->textures->Load("guile_stage.png");

	// TODO 7: Enable the player module
	App->player->Enable();
	App->enemy->Enable();
	App->projectile->Enable();

	// TODO 0: trigger background music
	App->audio->PlayMusic("guile.ogg");

	// Set camera limit
	App->renderer->cameraLimit = 270;

	return true;
}

// UnLoad assets
bool ModuleSceneGuile::CleanUp()
{
	LOG("Unloading ken scene");

	App->textures->Unload(graphics);
	App->player->Disable();
	App->enemy->Disable();
	App->projectile->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneGuile::Update()
{
	// Renders
	App->renderer->Blit(graphics, 0, 0, &sky, 1.0f);
	App->renderer->Blit(graphics, 0, 82, &background, 1.8f);
	App->renderer->Blit(graphics, 0, 0, &ground, SCREEN_SIZE);
	App->renderer->Blit(graphics, 0, 83, &fence, SCREEN_SIZE);

	App->renderer->Blit(graphics, 200, 85, &crowd_1.GetCurrentFrame(), SCREEN_SIZE);
	App->renderer->Blit(graphics, 300, 110, &crowd_2.GetCurrentFrame(), SCREEN_SIZE);
	App->renderer->Blit(graphics, 400, 100, &crowd_3.GetCurrentFrame(), SCREEN_SIZE);


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Switch too honda module
		App->fade->FadeToBlack(App->stage_selector, App->scene_guile, 3.0f);
	}

	SetCharactersLimit();

	if (!match_started && start_timer > 2.0f)
	{
		match_started = true;
		App->player->StartMatch();
		App->enemy->StartMatch();
	}
	else
	{
		start_timer += App->delta;
	}

	// Check if any of the players wins
	if (!App->player->is_alive)
	{
		if (end_timer > 0.5f)
		{
			App->enemy->WinMatch();
		}

		if (!is_fading && end_timer > 3.0f)
		{
			is_fading = true;
			App->fade->FadeToBlack(App->stage_selector, App->scene_guile, 3.0f);
		}

		end_timer += App->delta;
	}
	else if (!App->enemy->is_alive)
	{
		if (end_timer > 0.5f)
		{
			App->player->WinMatch();
		}

		if (!is_fading && end_timer > 3.0f)
		{
			is_fading = true;
			App->fade->FadeToBlack(App->stage_selector, App->scene_guile, 3.0f);
		}

		end_timer += App->delta;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneGuile::SetCharactersLimit()
{
	App->player->SetPositionLimits(-App->renderer->camera.x, -App->renderer->camera.x + SCREEN_WIDTH);
	App->enemy->SetPositionLimits(-App->renderer->camera.x, -App->renderer->camera.x + SCREEN_WIDTH);
}