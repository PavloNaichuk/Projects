#pragma once

#include "Mario.h"
#include "Enemy.h"
#include "FireBall.h"
#include "GoldCoin.h"
#include "PowerUpToLevel2.h"
#include "PowerUpToLevel3.h"
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
	std::vector<GoldCoin> mGoldCoins;
	std::vector<PowerUpToLevel2> mPowerUpToLevel2;
	std::vector<PowerUpToLevel3> mPowerUpToLevel3;
};
