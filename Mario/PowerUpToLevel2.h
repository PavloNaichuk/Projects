#pragma once

#include "Common.h"
#include "GameUnit.h"

struct GameWorld;

class PowerUpToLevel2 : public GameUnit
{
public:
	PowerUpToLevel2(State state, const Point& center, const Vector& velocity, const Size& size, int scores);
	void Update(float elapsedTime, GameWorld& gameWorld);

	bool mIsActive;
	int mScores;
};
