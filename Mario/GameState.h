#pragma once

#include <SDL.h>

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void ProcessKeyboard(SDL_Keycode key) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;	
};