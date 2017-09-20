#pragma once

#include "GameUnit.h"

struct GameWorld;

class Enemy : public GameUnit
{
public:
	Enemy(State state, const Point& center, const Vector& velocity, const Size& size, int scores);
	void Update(float elapsedTime, GameWorld& gameWorld);
	
	int mScores;
	bool mIsActive;
};