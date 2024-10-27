#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "ModuleCharacter.h"
#include "ModuleProjectile.h"
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
	void DrawToScreen();

	void Hit(const iPoint& position, const int area) override;
	void ThrowProjectile();
	void WinMatch() override;

private:
	ModuleProjectile* projectile;
	Animation projectile_anim;
};

#endif // __MODULEPLAYER_H__