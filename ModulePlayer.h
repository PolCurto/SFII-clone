#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;

enum PlayerState
{
	IDLE = 0,
	MOVEMENT,
	COMBAT
};

enum AttackState
{
	L_PUNCH,
	M_PUNCH
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void CheckPlayerInputs();
	void Move();

	void DrawPlayer();

public:

	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation backward;
	Animation forward;
	Animation light_punch;
	Animation medium_punch;
	iPoint position;

private:
	float speed;
	PlayerState state;
	AttackState attackState;
};

#endif // __MODULEPLAYER_H__