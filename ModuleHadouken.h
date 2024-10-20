#ifndef __MODULEHADOUKEN_H__
#define __MODULEHADOUKEN_H__

#include "Module.h"
#include "Globals.h"
#include "Animation.h"
#include "Point.h"
#include "Hitbox.h"

class ModuleCharacter;

struct SDL_Texture;

class ModuleHadouken : public Module
{
public:
	ModuleHadouken(bool start_enabled = false);
	~ModuleHadouken();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Move();
	void Despawn();
	void SetPosition();
	void CheckCollisions();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect sprite;
	Animation despawn;
	bool finished = false;
	bool isFlipped = false;

private:
	float time_to_kill;
	float timer;
	iPoint position;
	Hitbox hitbox;
	ModuleCharacter* enemy;
};

#endif // __MODULEHADOUKEN_H__