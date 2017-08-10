#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

class IGameState
{
public:
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual void processKeyboard(SDL_Keycode key) = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual ~IGameState() {}	
};