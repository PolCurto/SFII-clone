#pragma once

#include "SDL/include/SDL.h"
#include "Application.h"
#include "Module.h"
#include "ModuleFadeToBlack.h"
#include "Point.h"

struct SDL_Texture;

class StageIcon
{
public:
	Module* stage_to_load;
	SDL_Rect selected_icon;
	SDL_Rect unselected_icon;
	SDL_Rect current_icon;
	iPoint position;

public:
	StageIcon() : stage_to_load(nullptr), selected_icon({ 0, 0, 0, 0 }), unselected_icon({ 0, 0, 0, 0 }), current_icon(), position()
	{
		current_icon = unselected_icon;
	}

	void Select()
	{
		current_icon = selected_icon;
	}

	void UnSelect()
	{
		current_icon = unselected_icon;
	}
};
