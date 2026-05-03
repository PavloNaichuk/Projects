#pragma once

#include <SDL.h>

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void ProcessKeyPressed(SDL_Keycode key) = 0;
	virtual void ProcessKeyReleased(SDL_Keycode key) = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render() = 0;	
};