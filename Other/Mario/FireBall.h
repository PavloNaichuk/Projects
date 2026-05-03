#pragma once

#include "GameUnit.h"

struct GameWorld;

class FireBall : public GameUnit
{
public:
	FireBall(State state, const Point& center, const Vector& velocity, const Size& size);
	void Update(float elapsedTime, GameWorld& gameWorld);
	
	bool mIsActive;

private:
	void TestCollisionVersusEnemy(GameWorld& gameWorld);

	bool mCollidedWithGround;
};