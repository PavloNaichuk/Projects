#include "GameUnit.h"

GameUnit::GameUnit(State state, const Point& center, const Vector& velocity, const Size& size)
	: mState(state)
	, mCenter(center)
	, mVelocity(velocity)
	, mHalfSize(0.5f * size)
{
}
