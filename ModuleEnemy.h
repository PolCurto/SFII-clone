#ifndef __MODULEENEMY_H__
#define __MODULEPENEMY_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;

enum EnemyState
{
	EIDLE = 0,
	EMOVEMENT,
	ECOMBAT
};

enum EnemyAttackState
{
	EL_PUNCH,
	EM_PUNCH
};

class ModuleEnemy : public Module
{
public:
	ModuleEnemy(bool start_enabled = true);
	~ModuleEnemy();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Move();

	void DrawEnemy();

public:

	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation backward;
	Animation forward;
	Animation light_punch;
	Animation medium_punch;
	iPoint position;
	int positionLimit;

private:
	float speed;
	EnemyState state = EIDLE;
	EnemyAttackState attackState;
};

#endif // __MODULEENEMY_H__