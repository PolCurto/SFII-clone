#ifndef __MODULESTAGESELECTOR_H__
#define __MODULESTAGESELECTOR_H__

#include <vector>
#include "Module.h"
#include "Animator.h"
#include "Globals.h"

struct SDL_Texture;
class MenuSelection;


class ModuleStageSelector : public Module
{
public:
	ModuleStageSelector(bool start_enabled = true);
	~ModuleStageSelector();

	bool Start();
	update_status Update();
	bool CleanUp();

	Animator CreateRyuAnimator() const;
	Animator CreateKenAnimator() const;
	Animator CreateGuileAnimator() const;

public:

	SDL_Texture* graphics = nullptr;
	SDL_Rect background;
	SDL_Rect player_selector;
	vector<MenuSelection*> characters;
	int selected_character;
};

#endif // __MODULESTAGESELECTOR_H__