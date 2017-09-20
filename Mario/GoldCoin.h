#pragma once

#include "Common.h"
#include "GameUnit.h"

struct GameWorld;

class GoldCoin : public GameUnit
{
public:
	GoldCoin(State state, const Point& center, const Vector& velocity, const Size& size, int scores);
	void Update(float elapsedTime, GameWorld& gameWorld);

	bool mIsActive;
	int mScores;
};

