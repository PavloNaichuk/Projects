#pragma once

#include "Common.h"
#include "GameUnit.h"

class Mario : public GameUnit
{
public:
	Mario(State state, const Point& center, const Vector& velocity, const Size& size);

	void ProcessKeyPressed(SDL_Keycode key);
	void ProcessKeyReleased(SDL_Keycode key);
	void Update(float elapsedTime);
};
