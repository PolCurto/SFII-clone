#ifndef __MODULEENEMY_H__
#define __MODULEPENEMY_H__

#include "ModuleCharacter.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;


class ModuleEnemy : public ModuleCharacter
{
public:
	ModuleEnemy(bool start_enabled = true);
	~ModuleEnemy();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	void Move() override;
};

#endif // __MODULEENEMY_H__