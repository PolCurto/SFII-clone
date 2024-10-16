#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "ModuleCharacter.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;


class ModulePlayer : public ModuleCharacter
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	void Move() override;
	void CheckPlayerInputs();
};

#endif // __MODULEPLAYER_H__