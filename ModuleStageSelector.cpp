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
#include "CharacterData.h"

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

	// Ryu character
	ryu = new CharacterData("ryu4.png", CreateRyuAnimator(), {0, 0, 0, 0});
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

	App->player->character_data = *ryu;
	
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

Animator ModuleStageSelector::CreateRyuAnimator() const
{
	Animator ryu_animator;

	// idle animation (arcade sprite sheet)
	Animation idle;
	idle.frames.push_back({ 7, 14, 60, 90 });
	idle.frames.push_back({ 95, 15, 60, 89 });
	idle.frames.push_back({ 184, 14, 60, 90 });
	idle.frames.push_back({ 276, 11, 60, 93 });
	idle.frames.push_back({ 366, 12, 60, 92 });
	idle.speed = 10.0f;
	idle.loop = true;
	ryu_animator.AddAnimation("idle", idle);

	// walk backward animation (arcade sprite sheet)
	Animation backward;
	backward.frames.push_back({ 542, 131, 61, 87 });
	backward.frames.push_back({ 628, 129, 59, 90 });
	backward.frames.push_back({ 713, 128, 57, 90 });
	backward.frames.push_back({ 797, 127, 57, 90 });
	backward.frames.push_back({ 883, 128, 58, 91 });
	backward.frames.push_back({ 974, 129, 57, 89 });
	backward.speed = 12.0f;
	backward.loop = true;
	ryu_animator.AddAnimation("walk_b", backward);

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
	ryu_animator.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 13, 272, 70, 91 });
	light_punch.frames.push_back({ 98, 272, 126, 91 });
	light_punch.frames.push_back({ 13, 272, 70, 91 });
	light_punch.speed = 10.0f;
	light_punch.loop = false;
	ryu_animator.AddAnimation("light_punch", light_punch);

	// Medium punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 283, 269, 138, 94 });
	medium_punch.frames.push_back({ 453, 268, 138, 93 });
	medium_punch.frames.push_back({ 611, 268, 138, 92 });
	medium_punch.frames.push_back({ 453, 268, 138, 93 });
	medium_punch.frames.push_back({ 283, 269, 138, 94 });
	medium_punch.speed = 8.0f;
	medium_punch.loop = false;
	ryu_animator.AddAnimation("medium_punch", medium_punch);

	// Hadouken animation
	Animation hadouken_anim;
	hadouken_anim.frames.push_back({ 18, 1545, 90, 90 });
	hadouken_anim.frames.push_back({ 176, 1551, 116, 86 });
	hadouken_anim.frames.push_back({ 379, 1552, 102, 87 });
	hadouken_anim.frames.push_back({ 609, 1558, 118, 77 });
	hadouken_anim.frames.push_back({ 609, 1558, 118, 77 });
	hadouken_anim.speed = 10.0f;
	hadouken_anim.loop = false;
	ryu_animator.AddAnimation("hadouken", hadouken_anim);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 500, 2479, 42, 83 });
	victory.frames.push_back({ 577, 2474, 56, 88 });
	victory.frames.push_back({ 660, 2465, 60, 97 });
	victory.frames.push_back({ 745, 2440, 55, 122 });
	victory.speed = 6.0f;
	victory.loop = false;
	ryu_animator.AddAnimation("victory", victory);

	// Hurt animation
	Animation hurt;
	hurt.frames.push_back({ 398, 2094, 58, 85 });
	hurt.frames.push_back({ 482, 2097, 66, 82 });
	hurt.frames.push_back({ 30, 2101, 68, 79 });
	hurt.frames.push_back({ 117, 2090, 62, 90 });
	hurt.speed = 10.0f;
	hurt.loop = false;
	ryu_animator.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 350, 2233, 124, 63 });
	die.frames.push_back({ 687, 2247, 119, 44 });
	die.frames.push_back({ 849, 2246, 123, 41 });
	die.frames.push_back({ 984, 2265, 128, 31 });
	die.speed = 6.0f;
	die.loop = false;
	ryu_animator.AddAnimation("die", die);

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
	ryu_animator.AddAnimation("start", start);

	// Set the starting animation;
	ryu_animator.SetDefaultAnimation("idle");

	return ryu_animator;
}

Animator ModuleStageSelector::CreateKenAnimator() const
{
	return Animator();
}

Animator ModuleStageSelector::CreateGuileAnimator() const
{
	return Animator();
}
