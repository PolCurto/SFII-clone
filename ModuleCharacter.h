#ifndef __MODULECHARACTER_H__
#define __MODULECHARACTER_H__

#include "Module.h"
#include "Animator.h"
#include "Globals.h"
#include "Point.h"
#include "Hitbox.h"

struct SDL_Texture;
class ModuleEnemy;

enum CharacterState
{
	IDLE,
	MOVEMENT,
	COMBAT,
	HURT,
	DEAD,
	VICTORY,
	START
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

	virtual bool Start();
	virtual update_status Update();
	virtual bool CleanUp() { return true; };

	virtual void Move();
	virtual void StartMatch();
	virtual void SetPositionLimits(int min, int max);
	virtual void Hit(iPoint position, int area) {}
	virtual void TakeDamage(int damage);
	virtual void WinMatch();

public:
	SDL_Texture* graphics = nullptr;
	Animator animator;
	iPoint position;
	int maxPositionLimit;
	int minPositionLimit;
	Hitbox hitbox;
	bool is_alive;

protected:
	float speed = 0.0f;
	CharacterState state;
	AttackState attackState;
	bool is_flipped;
	Hitbox punch_box;

	SDL_Rect debugRect;
	int life;
	bool is_hurt;
	bool is_enabled;
};

#endif // __MODULECHARACTER_H__