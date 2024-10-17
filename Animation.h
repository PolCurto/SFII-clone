#pragma once
#include <vector>

using namespace std;

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;
	bool finished;
	bool started;

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), finished(false), started(false), current_frame(0.0f)
	{}

	SDL_Rect& GetCurrentFrame(bool loop)
	{
		// Starts the animation always at frame 0
		if (!started)
		{
			started = true;
			current_frame = 0.0f;
		}

		// If the animation has finished stops increasing the frame number
		if (!finished) current_frame += speed;

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
				current_frame = frames.size() - 1;
				finished = true;
			}
		}

		return frames[(int)current_frame];
	}

	SDL_Rect& GetCurrentFrameNum(bool loop, int& frameNum)
	{
		frameNum = (int)current_frame;

		// Starts the animation always at frame 0
		if (!started)
		{
			started = true;
			current_frame = 0.0f;
		}

		// If the animation has finished stops increasing the frame number
		if (!finished) current_frame += speed;

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
				current_frame = frames.size() - 1;
				finished = true;
			}
		}

		return frames[(int)current_frame];
	}
};