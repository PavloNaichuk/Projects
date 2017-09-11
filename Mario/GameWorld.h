#pragma once

#include "Mario.h"
#include "Enemy.h"
#include "FireBall.h"
#include "Config.h"
#include <vector>

struct GameWorld 
{
public:
	GameWorld();
	void Update(float elapsedTime);

	Mario mMario;
	std::vector<Enemy> mEnemies;
	std::vector<FireBall> mFireBalls;
};
