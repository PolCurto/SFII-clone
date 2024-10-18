#include "Globals.h"
#include "Application.h"
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

ModuleSceneHonda::ModuleSceneHonda(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 8;
	ground.y = 376;
	ground.w = 896;
	ground.h = 64;

	// Background
	background.x = 120;
	background.y = 129;
	background.w = 664;
	background.h = 198;

	// Roof
	roof.x = 91;
	roof.y = 8;
	roof.w = 756;
	roof.h = 48;

	// Bath
	bath.x = 164;
	bath.y = 66;
	bath.w = 336;
	bath.h = 51;

	// Barrel
	barrel.x = 542;
	barrel.y = 85;
	barrel.w = 32;
	barrel.h = 26;

	// Water animation
	water.frames.push_back({ 586, 448, 284, 8 });
	water.frames.push_back({ 8, 448, 284, 18 });
	water.frames.push_back({ 296, 448, 284, 12 });
	water.speed = 0.02f;

	// Mountains animation
	mountains.frames.push_back({ 224, 64, 224, 104 });
	mountains.frames.push_back({ 456, 64, 224, 104 });
	mountains.speed = 0.02f;

	// Face animation
	face.frames.push_back({ 688, 64, 95, 104 });
	face.frames.push_back({ 790, 64, 95, 104 });
	face.speed = 0.02f;
	

}

ModuleSceneHonda::~ModuleSceneHonda()
{}

// Load assets
bool ModuleSceneHonda::Start()
{
	LOG("Loading honda scene");

	graphics = App->textures->Load("honda_stage2.png");
	animation = App->textures->Load("honda_stage.png");

	// Set camera limit
	App->renderer->cameraLimit = 240;

	// TODO 7: Enable the player module
	App->player->Enable();
	App->enemy->Enable();

	// TODO 0: trigger background music
	App->audio->PlayMusic("honda.ogg");

	return true;
}

// UnLoad assets
bool ModuleSceneHonda::CleanUp()
{
	LOG("Unloading ken scene");

	App->textures->Unload(graphics);
	App->textures->Unload(animation);
	App->player->Disable();
	App->enemy->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneHonda::Update()
{
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, -100, 170, &ground, SCREEN_SIZE); // ground

	App->renderer->Blit(graphics, -10, 0, &background, SCREEN_SIZE); // background
	App->renderer->Blit(animation, 182, 56, &(mountains.GetCurrentFrame()), SCREEN_SIZE); // mountain animation
	App->renderer->Blit(animation, 405, 56, &(face.GetCurrentFrame()), SCREEN_SIZE); // face animation

	App->renderer->Blit(graphics, -60, 0, &roof, SCREEN_SIZE); // roof

	App->renderer->Blit(graphics, 175, 140, &bath, SCREEN_SIZE); // bath
	App->renderer->Blit(graphics, 203, 152, &(water.GetCurrentFrame()), SCREEN_SIZE); // water animation

	App->renderer->Blit(graphics, 90, 165, &barrel, SCREEN_SIZE); // barrel

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Switch too ken module
		App->fade->FadeToBlack(App->stage_selector, App->scene_honda, 3.0f);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneHonda::SetCharactersLimit()
{

}