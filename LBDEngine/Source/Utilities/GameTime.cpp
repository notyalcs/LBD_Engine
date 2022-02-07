#include "../../Headers/Utilities/GameTime.h"

float GameTime::_deltaTime{ 0.0f };
float GameTime::_totalTime{ 0.0f };
float GameTime::_secondsPerCount{ 0.0f };
INT64 GameTime::_initialTime{ 0 };
INT64 GameTime::_previousFrameTime{ 0 };
INT64 GameTime::_currentFrameTime{ 0 };

void GameTime::Initialize()
{
	INT64 countsPerSecond{};
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSecond));
	_secondsPerCount = 1.0f / static_cast<float>(countsPerSecond);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currentFrameTime));

	_initialTime = _currentFrameTime;
	_previousFrameTime = _currentFrameTime;
}

void GameTime::Update()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currentFrameTime));

	_deltaTime = (_currentFrameTime - _previousFrameTime) * _secondsPerCount;
	_previousFrameTime = _currentFrameTime;
	_totalTime = (_currentFrameTime - _initialTime) * _secondsPerCount;
}
