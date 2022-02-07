#pragma once
#include "Utilities.h"

/*
* GameTime keeps track of the important "delta time" (the time between frames) as well as the total time.
*/
struct GameTime
{
	GameTime() = delete;
	GameTime(const GameTime& rhs) = delete;
	void operator=(const GameTime& rhs) = delete;

	static float GetDeltaTime()
	{
		return _deltaTime;
	}
	
	static float GetTotalTime()
	{
		return _totalTime;
	}
	

	static void Initialize();
	static void Update();

private:
	// The time it took to render the last frame.
	static float _deltaTime;
	// The time since initialization.
	static float _totalTime;

	static float _secondsPerCount;
	
	static INT64 _initialTime;
	static INT64 _previousFrameTime;
	static INT64 _currentFrameTime;
};
