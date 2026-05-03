#pragma once

#include "Common.h"
#include "Trigger.h"

struct GameWorld;

class GoldCoin : public Trigger
{
public:
	GoldCoin(bool isActive, const Point& center, const Size& size, int scores);
	void Update(float elapsedTime, GameWorld& gameWorld);

	int mScores;
};

