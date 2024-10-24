#include "Globals.h"
#include "Application.h"
#include "ModuleSceneKen.h"
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

ModuleSceneKen::ModuleSceneKen(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;

	// TODO 2 : setup the foreground (red ship) with
	// coordinates x,y,w,h from ken_stage.png -- Done
	red_ship.x = 9;
	red_ship.y = 25;
	red_ship.w = 520;
	red_ship.h = 180;

	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;

	// flag animation
	flag.frames.push_back({848, 208, 40, 40});
	flag.frames.push_back({848, 256, 40, 40});
	flag.frames.push_back({848, 304, 40, 40});
	flag.speed = 8.0f;

	// TODO 4: Setup Girl Animation from coordinates from ken_stage.png

	// We add each frame specifying the origion x and y in the ken.png, and its width and length
	girl.frames.push_back({ 624, 16, 32, 56 });
	girl.frames.push_back({ 624, 80, 32, 56 });
	girl.frames.push_back({ 624, 144, 32, 56 });
	girl.frames.push_back({ 624, 80, 32, 56 });
	girl.speed = 4.0f;
}

ModuleSceneKen::~ModuleSceneKen()
{}

// Load assets
bool ModuleSceneKen::Start()
{
	LOG("Loading ken scene");

	is_fading = false;
	match_started = false;
	start_timer = 0;
	end_timer = 0;
	
	graphics = App->textures->Load("ken_stage.png");

	// TODO 7: Enable the player module
	App->player->Enable();
	App->enemy->Enable();

	// TODO 0: trigger background music
	App->audio->PlayMusic("guile.ogg");

	// Set camera limit
	App->renderer->cameraLimit = 500;

	return true;
}

// UnLoad assets
bool ModuleSceneKen::CleanUp()
{
	LOG("Unloading ken scene");

	App->textures->Unload(graphics);
	App->player->Disable();
	App->enemy->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneKen::Update()
{
	// TODO 5: make sure the ship goes up and down
	// Probably not the best method, but works
	ship_y += ship_sum;
	if (abs(ship_y) > 4) ship_sum *= -1;

	// Draw everything --------------------------------------
	// TODO 1: Tweak the movement speed of the sea&sky + flag to your taste -- Done
	App->renderer->Blit(graphics, 0, 0, &background, 2.0f); // sea and sky
	App->renderer->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), 2.0f); // flag animation

	// TODO 3: Draw the ship. Be sure to tweak the speed.
	App->renderer->Blit(graphics, 0, (int)-ship_y, &red_ship, 2.5f); // red ship

	// TODO 6: Draw the girl. Make sure it follows the ship movement!
	App->renderer->Blit(graphics, 191, 103 - (int)ship_y, &(girl.GetCurrentFrame()), 2.5f);
	
	App->renderer->Blit(graphics, 0, 170, &ground, SCREEN_SIZE);

	// TODO 10: Build an entire new scene "honda", you can find its
	// and music in the Game/ folder

	// TODO 11: Make that pressing space triggers a switch to honda logic module -- Donee
	// using FadeToBlack module
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Switch too honda module
		App->fade->FadeToBlack(App->stage_selector, App->scene_ken, 3.0f);
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
			App->fade->FadeToBlack(App->stage_selector, App->scene_ken, 3.0f);
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
			App->fade->FadeToBlack(App->stage_selector, App->scene_ken, 3.0f);
		}

		end_timer += App->delta;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneKen::SetCharactersLimit()
{
	App->player->SetPositionLimits(-App->renderer->camera.x, -App->renderer->camera.x + SCREEN_WIDTH);
	App->enemy->SetPositionLimits(-App->renderer->camera.x, -App->renderer->camera.x + SCREEN_WIDTH);
}