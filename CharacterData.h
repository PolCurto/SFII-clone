#pragma once

#include "SDL/include/SDL.h"
#include "Animator.h"
#include "ModuleProjectile.h"

class CharacterData
{
public:
	char* texture_name;
	Animator animator;
	Animator projectile_animator;

public:
	CharacterData() : texture_name(), animator()
	{}
	
	CharacterData(char* n, Animator a, Animator p) : texture_name(n), animator(a), projectile_animator(p)
	{}

};
