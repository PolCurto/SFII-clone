#include "Globals.h"
#include "Application.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneHonda.h"
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

ModuleStageSelector::ModuleStageSelector(bool start_enabled) : Module(start_enabled)
{
	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;
}

ModuleStageSelector::~ModuleStageSelector()
{}

// Load assets
bool ModuleStageSelector::Start()
{
	LOG("Loading stage selector");

	graphics = App->textures->Load("ken_stage.png");

	// TODO 0: trigger background music
	App->audio->PlayMusic("ken.ogg");

	return true;
}

// UnLoad assets
bool ModuleStageSelector::CleanUp()
{
	LOG("Unloading stage selector");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModuleStageSelector::Update()
{
	App->renderer->Blit(graphics, 0, 0, &background);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		// Switch too honda module
		App->fade->FadeToBlack(App->scene_ken, App->stage_selector, 3.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		// Switch too honda module
		App->fade->FadeToBlack(App->scene_honda, App->stage_selector, 3.0f);
	}

	return UPDATE_CONTINUE;
}