#pragma once

#include <SDL.h>

class EndMenuStateListener 
{
public:
	virtual ~EndMenuStateListener() {}

	virtual void OnGameOver() = 0;
};