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
	SDL_Rect selected_icon;
	SDL_Rect unselected_icon;
	SDL_Rect current_icon;
	iPoint position;

public:
	StageIcon() : selected_icon(), unselected_icon(), current_icon(), position()
	{}
	
	StageIcon(SDL_Rect selected, SDL_Rect unselected, SDL_Rect current, iPoint pos) 
		: selected_icon(selected), unselected_icon(unselected), current_icon(current), position(pos)
	{}
};
