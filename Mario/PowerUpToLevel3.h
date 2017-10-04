#pragma once

#include "Common.h"
#include "Trigger.h"

struct GameWorld;

class PowerUpToLevel3 : public Trigger
{
public:
	PowerUpToLevel3(bool isActive, const Point& center, const Size& size, int scores);
	void Update(float elapsedTime, GameWorld& gameWorld);

	int mScores;
};

