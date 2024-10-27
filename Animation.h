#pragma once
#include <vector>

using namespace std;

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;
	bool finished;
	bool loop;

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), finished(false), current_frame(0.0f), loop(true)
	{}

	SDL_Rect& GetCurrentFrame()
	{
		// If the animation has finished stops increasing the frame number
		if (!finished) current_frame += speed * App->delta;

		if (current_frame >= frames.size())
		{
			// If the animation loops resets the current frame
			if (loop)
			{
				current_frame = 0.0f;
			}
			// If the animation doesn't loop the frame number stays at the last and finishes
			else
			{	
				current_frame = static_cast<float>(frames.size() - 1);
				finished = true;
			}
		}

		return frames[(int)current_frame];
	}

	void Reset()
	{
		current_frame = 0.0f;
		finished = false;
	}

	int GetCurrentFrameNum() const
	{
		return (int)current_frame;
	}
};