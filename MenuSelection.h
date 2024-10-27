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
	bool selected;
	iPoint selector_position;

public:
	MenuSelection() : stage_icon(), character_data(), stage_to_load(), character_icon(), character_name(), selected(false), selector_position()
	{}

	MenuSelection(const StageIcon& icon, const CharacterData& data, const SDL_Rect& character, const SDL_Rect& name, const iPoint& position)
		: stage_icon(icon), character_data(data), stage_to_load(nullptr), character_icon(character), character_name(name), selected(false), selector_position(position)
	{}

	void Select()
	{
		selected = true;
		stage_icon.current_icon = stage_icon.selected_icon;
	}

	void UnSelect()
	{
		selected = false;
		stage_icon.current_icon = stage_icon.unselected_icon;
	}

};