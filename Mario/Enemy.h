#pragma once

#include "GameUnit.h"

class Enemy : public GameUnit
{
public:
	Enemy(State state, const Point& center, const Vector& velocity, const Size& size);
	void Update(float elapsedTime);
};