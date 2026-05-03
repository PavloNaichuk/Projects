#pragma once

#include <ctime>

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Enter() = 0;
	virtual void Update(std::time_t elapsedTimeInMS) = 0;
	virtual void Exit() = 0;
	virtual void Render() = 0;
	virtual void ProcessKeyPress(char key) = 0;
	virtual void ProcessKeyRelease(char key) = 0;
};