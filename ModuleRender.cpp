#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "SDL/include/SDL.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) debugCamera = !debugCamera;

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	// debug camera
	int speed = 1;
	if (debugCamera)
	{
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			App->renderer->camera.y += speed;

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			App->renderer->camera.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			App->renderer->camera.x += speed;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			App->renderer->camera.x -= speed;
	}
	else
	{
		CamFollowPlayer();
	}

	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, bool flip) const
{
	bool ret = true;
	SDL_Rect rect = {0, 0, 0, 0};
	rect.x = static_cast<int>(camera.x * speed) + x * SCREEN_SIZE;
	rect.y = static_cast<int>(camera.y * speed) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	SDL_RendererFlip renderFlip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	const SDL_Point point = { 0, 0 };

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, 0.0f, &point, renderFlip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void ModuleRender::CamFollowPlayer()
{
	camera.x = -((App->player->position.x + App->enemy->position.x) / 2) + (SCREEN_WIDTH / 2);
	camera.y = 0;

	if (camera.x > 0) camera.x = 0;
	else if (camera.x < -cameraLimit) camera.x = -cameraLimit;
}
