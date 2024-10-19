#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

using namespace std;

class Hitbox
{
public:
	SDL_Rect area;
	Module* parent;

public:
	Hitbox() : area({0, 0, 0, 0}), parent(nullptr)
	{}

	bool IsColliding(const SDL_Rect& other_hitbox) const
	{
		return SDL_HasIntersection(&area, &other_hitbox);
	}
};