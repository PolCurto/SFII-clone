#ifndef __MODULESCENEHONDA_H__
#define __MODULESCENEHONDA_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneHonda : public Module
{
public:
	ModuleSceneHonda(bool start_enabled = true);
	~ModuleSceneHonda();

	bool Start();
	update_status Update();
	bool CleanUp();

	void SetCharactersLimit() const;

public:

	SDL_Texture* graphics = nullptr;
	SDL_Texture* animation = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	SDL_Rect roof;
	SDL_Rect bath;
	SDL_Rect barrel;
	Animation mountains;
	Animation face;
	Animation water;

private:

	float start_timer = 0;
	float end_timer = 0;
	bool is_fading = false;
	bool match_started;
	
};

#endif // __MODULESCENEKEN_H__