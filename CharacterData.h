#pragma once

#include "SDL/include/SDL.h"
#include "Animator.h"

class CharacterData
{
public:
	char* texture_name;
	Animator animator;

public:
	CharacterData() : texture_name(), animator()
	{}
	
	CharacterData(char* n, Animator a) : texture_name(n), animator(a)
	{}

};
