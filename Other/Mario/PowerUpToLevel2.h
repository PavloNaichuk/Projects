#pragma once

#include "Common.h"
#include "Trigger.h"

struct GameWorld;

class PowerUpToLevel2 : public Trigger
{
public:
	PowerUpToLevel2(bool isActive, const Point& center, const Size& size, int scores);
	void Update(float elapsedTime, GameWorld& gameWorld);

	int mScores;
};
