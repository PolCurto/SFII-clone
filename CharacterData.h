#pragma once

#include "SDL/include/SDL.h"
#include "Animator.h"

struct SDL_Texture;

class CharacterData
{
public:
	char* texture_name;
	Animator animator;
	SDL_Rect icon;

public:
	CharacterData() : texture_name(), animator(), icon()
	{}
	
	CharacterData(char* n, Animator a, SDL_Rect i) : texture_name(n), animator(a), icon(i)
	{}

};
