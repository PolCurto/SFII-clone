#ifndef __MODULECHARACTER_H__
#define __MODULECHARACTER_H__

#include "Module.h"
#include "Animator.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;
class ModuleEnemy;

enum CharacterState
{
	IDLE,
	MOVEMENT,
	COMBAT
};

enum AttackState
{
	NO_ATTACK,
	L_PUNCH,
	M_PUNCH,
	HADOUKEN
};

class ModuleCharacter : public Module
{
public:
	ModuleCharacter(bool start_enabled = false);
	~ModuleCharacter();

	virtual bool Start() { return true; }
	virtual update_status Update();
	virtual bool CleanUp() { return true; };

	virtual void Move();

public:
	SDL_Texture* graphics = nullptr;
	Animator animator;
	iPoint position;
	int positionLimit;

protected:
	float speed = 0.0f;
	CharacterState state;
	AttackState attackState;
	bool isFlipped;
};

#endif // __MODULECHARACTER_H__