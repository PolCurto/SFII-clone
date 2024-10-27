#pragma once

#include "Animator.h"

class CharacterData
{
public:
	char* texture_name;
	Animator animator;
	Animator projectile_animator;

public:
	CharacterData() : texture_name(), animator(), projectile_animator()
	{}
	
	CharacterData(char* n, const Animator& a, const Animator& p) : texture_name(n), animator(a), projectile_animator(p)
	{}
};
