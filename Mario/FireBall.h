#pragma once

#include "GameUnit.h"

class FireBall : public GameUnit
{
public:
	FireBall(State state, const Point& center, const Vector& velocity, const Size& size);
	void Update(float elapsedTime);

private:
	bool mCollidedWithGround;
};