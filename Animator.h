#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "ModuleCharacter.h"
#include "Animation.h"

using namespace std;

class Animator
{
public:
	Animation* current_animation;

private:
	unordered_map<string, Animation> animations;

public:
	Animator() : animations(), current_animation()
	{}

	SDL_Rect& AnimateAction(const string &action)
	{
		// When the current animation is changed, resets the previous one
		if (current_animation != &animations[action] && current_animation != nullptr)
		{
			current_animation->Reset();
			current_animation = &animations[action];
		}

		return current_animation->GetCurrentFrame();
	}

	void AddAnimation(const string &name, const Animation &animation)
	{
		animations.insert({ name, animation });
	}

	void SetDefaultAnimation(const string &name)
	{
		current_animation = &animations[name];
	}

	bool AnimationFinished()
	{
		return current_animation->finished;
	}

	int GetCurrentFrameNum()
	{
		return current_animation->GetCurrentFrameNum();
	}
};