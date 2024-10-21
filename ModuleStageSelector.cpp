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
#include "StageIcon.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleStageSelector::ModuleStageSelector(bool start_enabled) : Module(start_enabled)
{
	// Background
	background.x = 30;
	background.y = 555;
	background.w = 384;
	background.h = 224;

	// USA Icon
	StageIcon usa;
	usa.position = { 290, 55 };
	usa.unselected_icon = { 111, 29, 32, 24 };
	usa.selected_icon = usa.current_icon = { 111, 63, 32, 24 };
	icons.push_back(usa);

	StageIcon japan;
	japan.position = { 195, 40 };
	japan.unselected_icon = japan.current_icon = { 70, 29, 32, 24 };
	japan.selected_icon = { 70, 63, 32, 24 };
	icons.push_back(japan);

	selected_icon = 0;
}

ModuleStageSelector::~ModuleStageSelector()
{}

// Load assets
bool ModuleStageSelector::Start()
{
	LOG("Loading stage selector");

	graphics = App->textures->Load("stage_select.png");

	icons.at(0).stage_to_load = App->scene_ken;
	icons.at(1).stage_to_load = App->scene_honda;

	// TODO 0: trigger background music
	//App->audio->PlayMusic("stage_select.ogg");

	App->renderer->cameraLimit = 0;
	
	(*icons.begin()).Select();

	for (vector<StageIcon>::iterator it = icons.begin() + 1; it != icons.end(); ++it)
	{
		(*it).UnSelect();
	}

	selected_icon = 0;

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

	for (vector<StageIcon>::iterator it = icons.begin(); it != icons.end(); ++it)
	{
		App->renderer->Blit(graphics, (*it).position.x, (*it).position.y, &(*it).current_icon);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		icons.at(selected_icon).UnSelect();

		selected_icon += 1;
		if (selected_icon >= icons.size()) selected_icon = 0;

		icons.at(selected_icon).Select();

	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		icons.at(selected_icon).UnSelect();

		selected_icon -= 1;
		if (selected_icon < 0) selected_icon = icons.size() - 1;

		icons.at(selected_icon).Select();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->fade->FadeToBlack(icons.at(selected_icon).stage_to_load, App->stage_selector, 3.0f);
	}

	return UPDATE_CONTINUE;
}