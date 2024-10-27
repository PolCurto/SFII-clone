#ifndef __MODULESCENEGUILE_H__
#define __MODULESCENEGUILE_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneGuile : public Module
{
public:
	ModuleSceneGuile(bool start_enabled = true);
	~ModuleSceneGuile();

	bool Start();
	update_status Update();
	bool CleanUp();

	void SetCharactersLimit();

public:

	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	SDL_Rect sky;
	SDL_Rect fence;
	Animation crowd_1;
	Animation crowd_2;
	Animation crowd_3;

private:

	float start_timer = 0;
	float end_timer = 0;
	bool is_fading = false;
	bool match_started;
};

#endif // __MODULESCENEGUILE_H__