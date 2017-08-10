#pragma once

#include <SDL.h>

class StartMenuStateListener
{
public:
	virtual ~StartMenuStateListener() {}

	virtual void OnGameStart() = 0;
};
