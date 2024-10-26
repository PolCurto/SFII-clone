#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include <list>
#include <chrono>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModulePlayer;
class ModuleEnemy;
class ModuleSceneKen;
class ModuleSceneHonda;
class ModuleStageSelector;
class ModuleProjectile;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModulePlayer* player;
	ModuleEnemy* enemy;
	ModuleSceneKen* scene_ken;
	ModuleSceneHonda* scene_honda;
	ModuleStageSelector* stage_selector;
	ModuleProjectile* projectile;

	double delta;

private:

	std::list<Module*> modules;
	std::chrono::steady_clock::time_point current_time;
	std::chrono::steady_clock::time_point last_time;
	std::chrono::duration<double, std::milli> time_lapse;
};

extern Application* App;

#endif // __APPLICATION_CPP__