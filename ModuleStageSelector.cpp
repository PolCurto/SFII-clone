#include "Globals.h"
#include "Application.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneHonda.h"
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
#include "MenuSelection.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleStageSelector::ModuleStageSelector(bool start_enabled) : Module(start_enabled)
{
	// Background
	background.x = 30;
	background.y = 555;
	background.w = 384;
	background.h = 224;

	player_selector = { 149, 97, 32, 36 };

	// Ryu
	StageIcon japan({ 70, 63, 32, 24 }, { 70, 29, 32, 24 }, { 70, 63, 32, 24 }, { 195, 40 });
	Animator ryu_animator, ryu_projectile_animator;
	CreateRyuAnimator(ryu_animator, ryu_projectile_animator);
	CharacterData ryu("ryu4.png", ryu_animator, ryu_projectile_animator);
	characters.push_back(new MenuSelection(japan, ryu, { 15, 153, 96, 96 }, { 40, 254, 47, 15 }, { 96, 140 }));

	// Guile
	StageIcon usa({ 111, 63, 32, 24 }, { 111, 29, 32, 24 }, { 111, 63, 32, 24 }, { 275, 35 });
	Animator guile_animator, guile_projectile_animator;
	CreateGuileAnimator(guile_animator, guile_projectile_animator);
	CharacterData guile("guile.png", guile_animator, guile_projectile_animator);
	characters.push_back(new MenuSelection(usa, guile, { 332, 153, 96, 96 }, { 343, 254, 74, 15 }, { 192, 140 }));

	// Ken
	StageIcon usa_2({ 111, 63, 32, 24 }, { 111, 29, 32, 24 }, { 111, 63, 32, 24 }, { 285, 70 });
	Animator ken_animator, ken_projectile_animator;
	CreateKenAnimator(ken_animator, ken_projectile_animator);
	CharacterData ken("ken.png", ken_animator, ken_projectile_animator);
	characters.push_back(new MenuSelection(usa_2, ken, { 15, 279, 96, 96 }, { 38, 380, 50, 15 }, { 96, 172 }));

	selected_character = 0;
}

ModuleStageSelector::~ModuleStageSelector()
{
	for (vector<MenuSelection*>::iterator it = characters.begin(); it != characters.end(); ++it)
		RELEASE(*it);
}

// Load assets
bool ModuleStageSelector::Start()
{
	LOG("Loading stage selector");

	graphics = App->textures->Load("stage_select.png");

	characters.at(0)->stage_to_load = App->scene_honda;
	characters.at(1)->stage_to_load = App->scene_guile;
	characters.at(2)->stage_to_load = App->scene_ken;

	App->audio->PlayMusic("stageselect.ogg");
	
	App->renderer->cameraLimit = 0;
	
	(*characters.begin())->Select();

	for (vector<MenuSelection*>::iterator it = characters.begin() + 1; it != characters.end(); ++it)
	{
		(*it)->UnSelect();
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
		if ((*it)->selected)
		{
			App->renderer->Blit(graphics, 0, SCREEN_HEIGHT - (*it)->character_icon.h, &(*it)->character_icon);
			App->renderer->Blit(graphics, (*it)->character_icon.w / 2 - ((*it)->character_name.w / 2), SCREEN_HEIGHT - (*it)->character_icon.h - 20, &(*it)->character_name);
			App->renderer->Blit(graphics, (*it)->selector_position.x, (*it)->selector_position.y, &player_selector);
		}
	}	
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		characters.at(selected_character)->UnSelect();

		selected_character += 1;
		if (selected_character >= characters.size()) selected_character = 0;

		characters.at(selected_character)->Select();

	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		characters.at(selected_character)->UnSelect();

		selected_character -= 1;
		if (selected_character < 0) selected_character = characters.size() - 1;

		characters.at(selected_character)->Select();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		App->player->character_data = characters.at(selected_character)->character_data;
		App->fade->FadeToBlack(characters.at(selected_character)->stage_to_load, App->stage_selector, 3.0f);
	}

	return UPDATE_CONTINUE;
}

void ModuleStageSelector::CreateRyuAnimator(Animator& character_anim, Animator& projectile_anim) const
{
	// idle animation (arcade sprite sheet)
	Animation idle;
	idle.frames.push_back({ 7, 14, 60, 90 });
	idle.frames.push_back({ 95, 15, 60, 89 });
	idle.frames.push_back({ 184, 14, 60, 90 });
	idle.frames.push_back({ 276, 11, 60, 93 });
	idle.frames.push_back({ 366, 12, 60, 92 });
	idle.speed = 10.0f;
	idle.loop = true;
	character_anim.AddAnimation("idle", idle);

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
	character_anim.AddAnimation("walk_b", backward);

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
	character_anim.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 13, 272, 70, 91 });
	light_punch.frames.push_back({ 98, 272, 126, 91 });
	light_punch.frames.push_back({ 13, 272, 70, 91 });
	light_punch.speed = 10.0f;
	light_punch.loop = false;
	character_anim.AddAnimation("light_punch", light_punch);

	// Medium punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 283, 269, 138, 94 });
	medium_punch.frames.push_back({ 453, 268, 138, 93 });
	medium_punch.frames.push_back({ 611, 268, 138, 92 });
	medium_punch.frames.push_back({ 453, 268, 138, 93 });
	medium_punch.frames.push_back({ 283, 269, 138, 94 });
	medium_punch.speed = 10.0f;
	medium_punch.loop = false;
	character_anim.AddAnimation("medium_punch", medium_punch);

	// Hadouken animation
	Animation hadouken_anim;
	hadouken_anim.frames.push_back({ 18, 1545, 90, 90 });
	hadouken_anim.frames.push_back({ 176, 1551, 116, 86 });
	hadouken_anim.frames.push_back({ 379, 1552, 102, 87 });
	hadouken_anim.frames.push_back({ 609, 1558, 118, 77 });
	hadouken_anim.frames.push_back({ 609, 1558, 118, 77 });
	hadouken_anim.speed = 10.0f;
	hadouken_anim.loop = false;
	character_anim.AddAnimation("hadouken", hadouken_anim);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 500, 2479, 42, 83 });
	victory.frames.push_back({ 577, 2474, 56, 88 });
	victory.frames.push_back({ 660, 2465, 60, 97 });
	victory.frames.push_back({ 745, 2440, 55, 122 });
	victory.speed = 6.0f;
	victory.loop = false;
	character_anim.AddAnimation("victory", victory);

	// Hurt animation
	Animation hurt;
	hurt.frames.push_back({ 398, 2094, 58, 85 });
	hurt.frames.push_back({ 482, 2097, 66, 82 });
	hurt.frames.push_back({ 30, 2101, 68, 79 });
	hurt.frames.push_back({ 117, 2090, 62, 90 });
	hurt.speed = 10.0f;
	hurt.loop = false;
	character_anim.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 350, 2233, 124, 63 });
	die.frames.push_back({ 687, 2247, 119, 44 });
	die.frames.push_back({ 849, 2246, 123, 41 });
	die.frames.push_back({ 984, 2265, 128, 31 });
	die.speed = 6.0f;
	die.loop = false;
	character_anim.AddAnimation("die", die);

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
	character_anim.AddAnimation("start", start);

	// Set the starting animation;
	character_anim.SetDefaultAnimation("idle");

	// Projectile
	Animation movement;
	movement.frames.push_back({ 819, 1563, 43, 32 });
	movement.speed = 0.0f;
	movement.loop = true;
	projectile_anim.AddAnimation("movement", movement);

	Animation despawn;
	despawn.frames.push_back({ 977, 1569, 26, 20});
	despawn.frames.push_back({ 1025, 1566, 26, 25});
	despawn.frames.push_back({ 1079, 1565, 30, 28});
	despawn.speed = 10.0f;
	despawn.loop = false;
	projectile_anim.AddAnimation("despawn", despawn);

	projectile_anim.SetDefaultAnimation("movement");
}

void ModuleStageSelector::CreateKenAnimator(Animator& character_anim, Animator& projectile_anim) const
{
	// idle animation (arcade sprite sheet)
	Animation idle;
	idle.frames.push_back({ 0, 1, 55, 93 });
	idle.frames.push_back({ 58, 2, 60, 92 });
	idle.frames.push_back({ 123, 4, 60, 90 });
	idle.frames.push_back({ 187, 5, 60, 89 });
	idle.frames.push_back({ 251, 4, 60, 90 });
	idle.speed = 10.0f;
	idle.loop = true;
	character_anim.AddAnimation("idle", idle);

	// Walk backward
	Animation backward;
	backward.frames.push_back({ 0, 1697, 64, 90 });
	backward.frames.push_back({ 85, 1697, 66, 90 });
	backward.frames.push_back({ 168, 1697, 72, 90 });
	backward.frames.push_back({ 258, 1696, 66, 91 });
	backward.frames.push_back({ 341, 1698, 62, 89 });
	backward.frames.push_back({ 427, 1700, 64, 87 });
	backward.speed = 12.0f;
	backward.loop = true;
	character_anim.AddAnimation("walk_b", backward);

	// Walk forward
	Animation forward;
	forward.frames.push_back({ 375, 1603, 66, 88 });
	forward.frames.push_back({ 461, 1601, 64, 90 });
	forward.frames.push_back({ 538, 1602, 78, 89 });
	forward.frames.push_back({ 630, 1602, 64, 89 });
	forward.frames.push_back({ 709, 1602, 58, 89 });
	forward.frames.push_back({ 779, 1608, 58, 83 });
	forward.speed = 12.0f;
	forward.loop = true;
	character_anim.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 390, 3, 78, 90 });
	light_punch.frames.push_back({ 480, 3, 134, 90 });
	light_punch.frames.push_back({ 390, 3, 78, 90 });
	light_punch.speed = 10.0f;
	light_punch.loop = false;
	character_anim.AddAnimation("light_punch", light_punch);

	// Medium punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 84, 103, 72, 94 });
	medium_punch.frames.push_back({ 192, 102, 74, 95 });
	medium_punch.frames.push_back({ 281, 103, 140, 94 });
	medium_punch.frames.push_back({ 192, 102, 74, 95 });
	medium_punch.speed = 8.0f;
	medium_punch.loop = false;
	character_anim.AddAnimation("medium_punch", medium_punch);

	// Hadouken animation
	Animation hadouken_anim;
	hadouken_anim.frames.push_back({ 497, 2550, 94, 90 });
	hadouken_anim.frames.push_back({ 606, 2556, 116, 84 });
	hadouken_anim.frames.push_back({ 654, 2445, 108, 81 });
	hadouken_anim.frames.push_back({ 810, 2514, 118, 77 });
	hadouken_anim.frames.push_back({ 810, 2514, 118, 77 });
	hadouken_anim.speed = 10.0f;
	hadouken_anim.loop = false;
	character_anim.AddAnimation("hadouken", hadouken_anim);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 480, 2424, 60, 89 });
	victory.frames.push_back({ 545, 2416, 60, 97 });
	victory.frames.push_back({ 0, 2518, 55, 122 });
	victory.speed = 6.0f;
	victory.loop = false;
	character_anim.AddAnimation("victory", victory);

	// Hurt animation
	Animation hurt;
	hurt.frames.push_back({ 518, 1697, 62, 90 });
	hurt.frames.push_back({ 609, 1696, 66, 91 });
	hurt.frames.push_back({ 714, 1699, 68, 88 });
	hurt.loop = false;
	hurt.speed = 10.0f;
	character_anim.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 134, 1799, 71, 78 });
	die.frames.push_back({ 845, 1833, 119, 44 });
	die.frames.push_back({ 717, 1836, 123, 41 });
	die.frames.push_back({ 584, 1846, 128, 31 });
	die.speed = 6.0f;
	die.loop = false;
	character_anim.AddAnimation("die", die);

	// Start animation
	Animation start;
	start.frames.push_back({ 124, 2541, 56, 99 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 251, 2545, 54, 95 });
	start.frames.push_back({ 251, 2545, 54, 95 });
	start.frames.push_back({ 251, 2545, 54, 95 });
	start.frames.push_back({ 251, 2545, 54, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.frames.push_back({ 185, 2545, 61, 95 });
	start.speed = 10.0f;
	start.loop = false;
	character_anim.AddAnimation("start", start);

	// Set the starting animation;
	character_anim.SetDefaultAnimation("idle");

	// Projectile
	Animation movement;
	movement.frames.push_back({ 643, 2044, 43, 32 });
	movement.speed = 0.0f;
	movement.loop = true;
	projectile_anim.AddAnimation("movement", movement);

	Animation despawn;
	despawn.frames.push_back({ 801, 2050, 26, 20 });
	despawn.frames.push_back({ 849, 2047, 26, 25 });
	despawn.frames.push_back({ 903, 2046, 30, 28 });
	despawn.speed = 10.0f;
	despawn.loop = false;
	projectile_anim.AddAnimation("despawn", despawn);

	projectile_anim.SetDefaultAnimation("movement");
}

void ModuleStageSelector::CreateGuileAnimator(Animator& character_anim, Animator& projectile_anim) const
{
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
	character_anim.AddAnimation("idle", idle);

	// walk backward animation (arcade sprite sheet)
	Animation backward;
	backward.frames.push_back({ 24, 232, 85, 96 });
	backward.frames.push_back({ 113, 229, 80, 100 });
	backward.frames.push_back({ 197, 228, 80, 102 });
	backward.frames.push_back({ 281, 227, 80, 102 });
	backward.frames.push_back({ 383, 233, 68, 95 });
	backward.speed = 12.0f;
	backward.loop = true;
	character_anim.AddAnimation("walk_b", backward);

	// TODO 8: setup the walk forward animation from ryu4.png -- Done
	Animation forward;
	forward.frames.push_back({ 536, 237, 84, 91 });
	forward.frames.push_back({ 636, 237, 68, 91 });
	forward.frames.push_back({ 721, 236, 58, 92 });
	forward.frames.push_back({ 798, 235, 58, 93 });
	forward.frames.push_back({ 872, 238, 60, 90 });
	forward.speed = 12.0f;
	forward.loop = true;
	character_anim.AddAnimation("walk_f", forward);

	// Light punch animation
	Animation light_punch;
	light_punch.frames.push_back({ 15, 565, 80, 91 });
	light_punch.frames.push_back({ 106, 565, 142, 91 });
	light_punch.frames.push_back({ 249, 565, 96, 91 });
	light_punch.speed = 10.0f;
	light_punch.loop = false;
	character_anim.AddAnimation("light_punch", light_punch);

	// Medium punch animation
	Animation medium_punch;
	medium_punch.frames.push_back({ 24, 731, 62, 93 });
	medium_punch.frames.push_back({ 109, 731, 90, 93 });
	medium_punch.frames.push_back({ 245, 730, 182, 94 });
	medium_punch.speed = 10.0f;
	medium_punch.loop = false;
	character_anim.AddAnimation("medium_punch", medium_punch);

	// Hadouken animation
	Animation hadouken_anim;
	hadouken_anim.frames.push_back({ 24, 2367, 76, 81 });
	hadouken_anim.frames.push_back({ 159, 2360, 130, 88 });
	hadouken_anim.frames.push_back({ 339, 2364, 146, 84 });
	hadouken_anim.frames.push_back({ 532, 2361, 188, 87 });
	hadouken_anim.speed = 10.0f;
	hadouken_anim.loop = false;
	character_anim.AddAnimation("hadouken", hadouken_anim);

	// Victory animation
	Animation victory;
	victory.frames.push_back({ 19, 3887, 78, 112 });
	victory.frames.push_back({ 100, 3887, 78, 112 });
	victory.frames.push_back({ 180, 3887, 78, 112 });
	victory.frames.push_back({ 271, 3882, 78, 117 });
	victory.frames.push_back({ 365, 3877, 78, 122 });
	victory.frames.push_back({ 471, 3877, 78, 122 });
	victory.frames.push_back({ 567, 3882, 78, 117 });
	victory.frames.push_back({ 18, 4060, 78, 112 });
	victory.frames.push_back({ 116, 4060, 104, 112 });
	victory.frames.push_back({ 230, 4060, 104, 112 });
	victory.frames.push_back({ 357, 4060, 98, 112 });
	victory.frames.push_back({ 492, 4052, 96, 120 });
	victory.frames.push_back({ 629, 4044, 88, 128 });
	victory.frames.push_back({ 753, 4024, 82, 148 });
	victory.frames.push_back({ 887, 4024, 82, 148 });
	victory.speed = 8.0f;
	victory.loop = false;
	character_anim.AddAnimation("victory", victory);

	// Hurt animation
	Animation hurt;
	hurt.frames.push_back({ 20, 3404, 72, 92 });
	hurt.frames.push_back({ 102, 3409, 70, 87 });
	hurt.frames.push_back({ 182, 3421, 94, 75 });
	hurt.frames.push_back({ 314, 3436, 100, 60 });
	hurt.frames.push_back({ 102, 3409, 70, 87 });
	hurt.speed = 10.0f;
	hurt.loop = false;
	character_anim.AddAnimation("hurt", hurt);

	// Defeat animation
	Animation die;
	die.frames.push_back({ 20, 3404, 72, 92 });
	die.frames.push_back({ 448, 3754, 126, 62 });
	die.frames.push_back({ 611, 3769, 122, 47 });
	die.frames.push_back({ 746, 3782, 139, 34 });
	die.speed = 6.0f;
	die.loop = false;
	character_anim.AddAnimation("die", die);

	// Start animation
	Animation start;
	start.frames.push_back({ 549, 4221, 80, 114 });
	start.frames.push_back({ 654, 4210, 76, 125 });
	start.frames.push_back({ 762, 4215, 66, 120 });
	start.frames.push_back({ 865, 4210, 66, 125 });
	start.frames.push_back({ 549, 4221, 80, 114 });
	start.frames.push_back({ 654, 4210, 76, 125 });
	start.frames.push_back({ 762, 4215, 66, 120 });
	start.frames.push_back({ 865, 4210, 66, 125 });
	start.frames.push_back({ 958, 4215, 86, 120 });
	start.frames.push_back({ 1066, 4215, 76, 120 });
	start.frames.push_back({ 1169, 4216, 76, 119 });
	start.frames.push_back({ 1272, 4220, 76, 115 });
	start.frames.push_back({ 1272, 4220, 76, 115 });
	start.speed = 7.0;
	start.loop = false;
	character_anim.AddAnimation("start", start);

	// Set the starting animation;
	character_anim.SetDefaultAnimation("idle");

	// Projectile
	// Projectile
	Animation movement;
	movement.frames.push_back({ 843, 2375, 63, 17 });
	movement.frames.push_back({ 1074, 2382, 59, 10 });
	movement.frames.push_back({ 922, 2375, 63, 17 });
	movement.frames.push_back({ 1000, 2382, 59, 17 });
	movement.speed = 10.0f;
	movement.loop = true;
	projectile_anim.AddAnimation("movement", movement);

	Animation despawn;
	despawn.frames.push_back({ 1150, 2372, 26, 20 });
	despawn.frames.push_back({ 1194, 2367, 26, 25 });
	despawn.frames.push_back({ 1209, 2367, 30, 28 });
	despawn.speed = 10.0f;
	despawn.loop = false;
	projectile_anim.AddAnimation("despawn", despawn);

	projectile_anim.SetDefaultAnimation("movement");
}
