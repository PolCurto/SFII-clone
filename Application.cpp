#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneHonda.h"
#include "ModuleStageSelector.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	// Game Modules
	modules.push_back(stage_selector = new ModuleStageSelector(false));
	modules.push_back(scene_ken = new ModuleSceneKen(false));
	modules.push_back(scene_honda = new ModuleSceneHonda(false));
	modules.push_back(player = new ModulePlayer(false));
	modules.push_back(hadouken = new ModuleHadouken(false));
	modules.push_back(enemy = new ModuleEnemy(false));
	modules.push_back(fade = new ModuleFadeToBlack());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	last_time = std::chrono::high_resolution_clock::now();
	current_time = last_time;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	fade->FadeToBlack(stage_selector, nullptr, 3.0f);

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	
	current_time = std::chrono::steady_clock::now();
	delta = current_time - last_time;
	LOG("first: %f", delta);
	last_time = std::chrono::steady_clock::now();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();
	
	/*
	auto currentTime = std::chrono::high_resolution_clock::now();

	// Calculate the frame duration (time between frames)
	std::chrono::duration<double> delta = currentFrameTime - previousFrameTime;
	frameDuration = delta.count();

	// Output the frame duration in seconds
	std::cout << "Frame duration: " << frameDuration << " seconds" << std::endl;

	// Update previous frame time for the next loop
	previousFrameTime = currentFrameTime;
	*/


	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

