#ifndef __MODULESTAGESELECTOR_H__
#define __MODULESTAGESELECTOR_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleStageSelector : public Module
{
public:
	ModuleStageSelector(bool start_enabled = true);
	~ModuleStageSelector();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	SDL_Rect background;

};

#endif // __MODULESTAGESELECTOR_H__