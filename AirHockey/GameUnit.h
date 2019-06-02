#pragma once

#include "Vector.h"

enum class State
{
	Standing,
	Moving,
	Jumping
};

struct GameUnit
{
public:
	GameUnit(State state, const Point& center, const Vector& velocity, const Size& size);

	State mState;
	Point mCenter;
	Vector mVelocity;
	Size mHalfSize;
};
