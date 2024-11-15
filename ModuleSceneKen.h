#ifndef __MODULESCENEKEN_H__
#define __MODULESCENEKEN_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneKen : public Module
{
public:
	ModuleSceneKen( bool start_enabled = true);
	~ModuleSceneKen();

	bool Start();
	update_status Update();
	bool CleanUp();

	void SetCharactersLimit() const;

public:
	
	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	SDL_Rect red_ship;
	Animation flag;
	Animation girl;

private:

	float ship_y = 0;
	float ship_sum = 0.1f;
	float start_timer = 0;
	float end_timer = 0;
	bool is_fading = false;
	bool match_started;
};

#endif // __MODULESCENEKEN_H__