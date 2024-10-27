#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

using namespace std;

class Hitbox
{
public:
	SDL_Rect area;
	Module parent;

public:
	Hitbox() : area(), parent()
	{}

	bool IsColliding(const Hitbox& other_hitbox) const
	{
		return SDL_HasIntersection(&area, &other_hitbox.area);
	}
};