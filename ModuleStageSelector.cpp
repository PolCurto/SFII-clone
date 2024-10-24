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
#include "MenuSelection.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleStageSelector::ModuleStageSelector(bool start_enabled) : Module(start_enabled)
{
	// Background
	background.x = 30;
	background.y = 555;
	background.w = 384;
	background.h = 224;

	SDL_Rect player_selector = { 149, 97, 32, 36 };

	// Ryu
	StageIcon japan({ 70, 63, 32, 24 }, { 70, 29, 32, 24 }, { 70, 63, 32, 24 }, { 195, 40 });
	CharacterData ryu("ryu4.png", CreateRyuAnimator());
	characters.push_back(new MenuSelection(japan, ryu, { 15, 153, 96, 96 }, { 40, 254, 47, 15}, player_selector));

	// Guile
	StageIcon usa({ 111, 63, 32, 24 }, { 111, 29, 32, 24 }, { 111, 63, 32, 24 }, { 290, 55 });
	CharacterData guile("guile.png", CreateGuileAnimator());
	characters.push_back(new MenuSelection(usa, guile, { 332, 153, 96, 96 }, { 343, 254, 74, 15 }, player_selector));

	selected_character = 0;

	// Ryu character
	//ryu = new CharacterData("ryu4.png", CreateRyuAnimator(), {0, 0, 0, 0});
}

ModuleStageSelector::~ModuleStageSelector()
{}

// Load assets
bool ModuleStageSelector::Start()
{
	LOG("Loading stage selector");

	graphics = App->textures->Load("stage_select.png");

	characters.at(0)->stage_to_load = App->scene_honda;
	characters.at(1)->stage_to_load = App->scene_ken;

	// TODO 0: trigger background music
	//App->audio->PlayMusic("stage_select.ogg");
	
	App->renderer->cameraLimit = 0;
	
	(*characters.begin())->stage_icon.Select();

	for (vector<MenuSelection*>::iterator it = characters.begin() + 1; it != characters.end(); ++it)
	{
		(*it)->stage_icon.UnSelect();
	}

	selected_character = 0;

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

	for (vector<MenuSelection*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		App->renderer->Blit(graphics, (*it)->stage_icon.position.x , (*it)->stage_icon.position.y, &(*it)->stage_icon.current_icon);
		App->renderer->Blit(graphics, 0, SCREEN_HEIGHT / 2, &(*it)->character_icon);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		characters.at(selected_character)->stage_icon.UnSelect();

		selected_character += 1;
		if (selected_character >= characters.size()) selected_character = 0;

		characters.at(selected_character)->stage_icon.Select();

	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		characters.at(selected_character)->stage_icon.UnSelect();

		selected_character += 1;
		if (selected_character >= characters.size()) selected_character = 0;

		characters.at(selected_character)->stage_icon.Select();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->player->character_data = characters.at(selected_character)->character_data;
		App->fade->FadeToBlack(characters.at(selected_character)->stage_to_load, App->stage_selector, 3.0f);
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

Animator ModuleStageSelector::CreateGuileAnimator() const
{
	Animator guile_animator;

	// idle animation (arcade sprite sheet)
	Animation idle;
	idle.frames.push_back({ 24, 62, 76, 90 });
	idle.frames.push_back({ 116, 63, 76, 89 });
	idle.frames.push_back({ 208, 62, 75, 90 });
	idle.frames.push_back({ 302, 62, 74, 90 });
	idle.frames.push_back({ 392, 62, 75, 90 });
	idle.frames.push_back({ 484, 62, 76, 90 });
	idle.speed = 10.0f;
	idle.loop = true;
	guile_animator.AddAnimation("idle", idle);

	// walk backward animation (arcade sprite sheet)
	Animation backward;
	backward.frames.push_back({ 24, 232, 85, 96 });
	backward.frames.push_back({ 113, 229, 80, 100 });
	backward.frames.push_back({ 197, 228, 80, 102 });
	backward.frames.push_back({ 281, 227, 80, 102 });
	backward.frames.push_back({ 383, 233, 68, 95 });
	backward.speed = 12.0f;
	backward.loop = true;
	guile_animator.AddAnimation("walk_b", backward);

	// TODO 8: setup the walk forward animation from ryu4.png -- Done
	Animation forward;
	forward.frames.push_back({ 536, 237, 84, 91 });
	forward.frames.push_back({ 636, 237, 68, 91 });
	forward.frames.push_back({ 721, 236, 58, 92 });
	forward.frames.push_back({ 798, 235, 58, 93 });
	forward.frames.push_back({ 872, 238, 60, 90 });
	forward.speed = 12.0f;
	forward.loop = true;
	guile_animator.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 15, 565, 80, 91 });
	light_punch.frames.push_back({ 106, 565, 142, 91 });
	light_punch.frames.push_back({ 249, 565, 96, 91 });
	light_punch.speed = 10.0f;
	light_punch.loop = false;
	guile_animator.AddAnimation("light_punch", light_punch);

	// Medium punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 24, 731, 62, 93 });
	medium_punch.frames.push_back({ 109, 731, 90, 93 });
	medium_punch.frames.push_back({ 245, 730, 182, 94 });
	medium_punch.speed = 8.0f;
	medium_punch.loop = false;
	guile_animator.AddAnimation("medium_punch", medium_punch);

	// Hadouken animation
	Animation hadouken_anim;
	hadouken_anim.frames.push_back({ 24, 2367, 76, 81 });
	hadouken_anim.frames.push_back({ 159, 2360, 130, 88 });
	hadouken_anim.frames.push_back({ 339, 2364, 146, 84 });
	hadouken_anim.frames.push_back({ 532, 2361, 188, 87 });
	hadouken_anim.speed = 10.0f;
	hadouken_anim.loop = false;
	guile_animator.AddAnimation("hadouken", hadouken_anim);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 500, 2479, 42, 83 });
	victory.frames.push_back({ 577, 2474, 56, 88 });
	victory.frames.push_back({ 660, 2465, 60, 97 });
	victory.frames.push_back({ 745, 2440, 55, 122 });
	victory.speed = 6.0f;
	victory.loop = false;
	guile_animator.AddAnimation("victory", victory);

	// Hurt animation
	Animation hurt;
	hurt.frames.push_back({ 398, 2094, 58, 85 });
	hurt.frames.push_back({ 482, 2097, 66, 82 });
	hurt.frames.push_back({ 30, 2101, 68, 79 });
	hurt.frames.push_back({ 117, 2090, 62, 90 });
	hurt.speed = 10.0f;
	hurt.loop = false;
	guile_animator.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 350, 2233, 124, 63 });
	die.frames.push_back({ 687, 2247, 119, 44 });
	die.frames.push_back({ 849, 2246, 123, 41 });
	die.frames.push_back({ 984, 2265, 128, 31 });
	die.speed = 6.0f;
	die.loop = false;
	guile_animator.AddAnimation("die", die);

	// Start animation
	Animation start;
	start.frames.push_back({ 549, 4221, 80, 114 });
	start.frames.push_back({ 654, 4210, 76, 125 });
	start.frames.push_back({ 762, 4215, 66, 120 });
	start.frames.push_back({ 865, 4210, 66, 125 });
	start.frames.push_back({ 958, 4215, 86, 120 });
	start.frames.push_back({ 1066, 4215, 76, 120 });
	start.frames.push_back({ 1169, 4216, 76, 119 });
	start.frames.push_back({ 1272, 4220, 76, 115 });
	start.frames.push_back({ 1272, 4220, 76, 115 });
	start.speed = 5.0;
	start.loop = false;
	guile_animator.AddAnimation("start", start);

	// Set the starting animation;
	guile_animator.SetDefaultAnimation("idle");

	return guile_animator;
}
