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
	SDL_Rect character_name;
	SDL_Rect player_selector;

public:
	MenuSelection() : stage_icon(), character_data(), stage_to_load(), character_icon(), character_name(), player_selector()
	{}

	MenuSelection(StageIcon icon, CharacterData data, SDL_Rect character, SDL_Rect name, SDL_Rect selector) 
		: stage_icon(icon), character_data(data), character_icon(character), character_name(name), player_selector(selector)
	{}

};