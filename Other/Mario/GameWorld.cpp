#include "GameWorld.h"


bool IsFireBallDestroyed(const FireBall& fireBall)
{
	return ((fireBall.mCenter.mX < fireBall.mHalfSize.mX) 
		|| (fireBall.mCenter.mX + fireBall.mHalfSize.mX > WINDOW_WIDTH) 
		|| !fireBall.mIsActive);
}

template <typename T>
bool IsNotActive(const T& gameObject)
{
	return !gameObject.mIsActive;
}

GameWorld::GameWorld()
	: mMario(State::Standing, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(MARIO_WIDTH, MARIO_HEIGHT))
{
}

void GameWorld::Update(float elapsedTime)
{
	mMario.Update(elapsedTime);

	for (FireBall& fireBall : mFireBalls)
		fireBall.Update(elapsedTime, *this);
	auto itFireBall = std::remove_if(mFireBalls.begin(), mFireBalls.end(), IsFireBallDestroyed);
	mFireBalls.erase(itFireBall, mFireBalls.end());

	for (Enemy& enemy : mEnemies)
		enemy.Update(elapsedTime, *this);
	auto itEnemy = std::remove_if(mEnemies.begin(), mEnemies.end(), IsNotActive<Enemy>);
	mEnemies.erase(itEnemy, mEnemies.end());

	for (GoldCoin& goldCoin : mGoldCoins)
		goldCoin.Update(elapsedTime, *this);
	auto itGoldCoin = std::remove_if(mGoldCoins.begin(), mGoldCoins.end(), IsNotActive<GoldCoin>);
	mGoldCoins.erase(itGoldCoin, mGoldCoins.end());

	for (PowerUpToLevel2& powerUpToLevel2 : mPowerUpToLevel2)
		powerUpToLevel2.Update(elapsedTime, *this);
	auto itPowerUpToLevel2 = std::remove_if(mPowerUpToLevel2.begin(), mPowerUpToLevel2.end(), IsNotActive<PowerUpToLevel2>);
	mPowerUpToLevel2.erase(itPowerUpToLevel2, mPowerUpToLevel2.end());

	for (PowerUpToLevel3& powerUpToLevel3 : mPowerUpToLevel3)
		powerUpToLevel3.Update(elapsedTime, *this);
	auto itPowerUpToLevel3 = std::remove_if(mPowerUpToLevel3.begin(), mPowerUpToLevel3.end(), IsNotActive<PowerUpToLevel3>);
	mPowerUpToLevel3.erase(itPowerUpToLevel3, mPowerUpToLevel3.end());
}