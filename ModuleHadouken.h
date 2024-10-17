#ifndef __MODULEHADOUKEN_H__
#define __MODULEHADOUKEN_H__

#include "Module.h"
#include "Globals.h"
#include "Animation.h"
#include "Point.h"

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
	void SetPosition(int x, int y);

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect sprite;
	Animation despawn;
	bool finished = false;
	bool isFlipped = false;

private:
	float timeToKill = 3.0f;
	float timer = 0.0f;
	iPoint position;
};

#endif // __MODULEHADOUKEN_H__