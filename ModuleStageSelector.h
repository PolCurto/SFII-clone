#ifndef __MODULESTAGESELECTOR_H__
#define __MODULESTAGESELECTOR_H__

#include <vector>
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "StageIcon.h"

struct SDL_Texture;
class CharacterData;
class Animator;


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
	vector<StageIcon> icons;
	int selected_icon;

	CharacterData* ryu;

};

#endif // __MODULESTAGESELECTOR_H__