#include "GameWorld.h"

bool IsFireBallDestroyed(const FireBall& fireBall)
{
	return ((fireBall.mCenter.mX < fireBall.mHalfSize.mX) || (fireBall.mCenter.mX + fireBall.mHalfSize.mX > WINDOW_WIDTH) || (!fireBall.mIsActive));
}

bool IsEnemyDestroyed(const Enemy& enemy)
{
	return !enemy.mIsActive;
}

bool IsGoldCoinDestroyed(const GoldCoin& goldCoin)
{
	return !goldCoin.mIsActive;
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
	auto itEnemy = std::remove_if(mEnemies.begin(), mEnemies.end(), IsEnemyDestroyed);
	mEnemies.erase(itEnemy, mEnemies.end());

	for (GoldCoin& goldCoin : mGoldCoins)
		goldCoin.Update(elapsedTime, *this);
	auto itGoldCoin = std::remove_if(mGoldCoins.begin(), mGoldCoins.end(), IsGoldCoinDestroyed);
	mGoldCoins.erase(itGoldCoin, mGoldCoins.end());
}