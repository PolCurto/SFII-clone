#ifndef __MODULEPPROJECTILE_H_
#define __MODULEPPROJECTILE_H_

#include "Module.h"
#include "Globals.h"
#include "Animator.h"
#include "Point.h"
#include "Hitbox.h"

class ModuleCharacter;

struct SDL_Texture;

class ModuleProjectile : public Module
{
public:
	ModuleProjectile(bool start_enabled = false);
	~ModuleProjectile();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Move();
	void Despawn();
	void SetPosition();
	void CheckCollisions();

public:
	SDL_Texture* graphics = nullptr;
	Animator animator;
	bool is_finished = false;
	bool is_flipped = false;

private:
	float time_to_kill;
	float timer;
	iPoint position;
	Hitbox hitbox;
	ModuleCharacter* enemy;
};

#endif // __MODULEPPROJECTILE_H_
