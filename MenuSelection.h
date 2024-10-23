#pragma once

#include "CharacterData.h"
#include "StageIcon.h"

class MenuSelection
{
public:
	StageIcon stage_icon;
	CharacterData character_data;
	Module* stage_to_load;
	SDL_Rect character_icon;

public:
	MenuSelection() : stage_icon(), character_data(), stage_to_load(), character_icon()
	{}

	MenuSelection(StageIcon icon, CharacterData data, SDL_Rect character) 
		: stage_icon(icon), character_data(data), character_icon(character)
	{}

};