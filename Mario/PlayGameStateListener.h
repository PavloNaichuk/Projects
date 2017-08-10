#pragma once

#include <SDL.h>

class PlayGameStateListener
{
public:
	virtual ~PlayGameStateListener() {}

	virtual void OnLevelLose() = 0;
	virtual void OnLevelComplete() = 0;
};
