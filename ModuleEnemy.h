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
	void DrawToScreen();

	void DoSomething();

private: 
	float timer = 0;
	float time_to_act = 2.0f;
};

#endif // __MODULEENEMY_H__