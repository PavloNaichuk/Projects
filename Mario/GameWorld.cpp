#include "GameWorld.h"

bool IsFireBallDestroyed(const FireBall& fireBall)
{
	return ((fireBall.mCenter.mX < fireBall.mHalfSize.mX) || (fireBall.mCenter.mX + fireBall.mHalfSize.mX > WINDOW_WIDTH));
}

GameWorld::GameWorld()
	: mMario(State::Standing, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(MARIO_WIDTH, MARIO_HEIGHT))
{
}

void GameWorld::Update(float elapsedTime)
{
	mMario.Update(elapsedTime);

	for (Enemy& enemy : mEnemies)
		enemy.Update(elapsedTime);

	for (FireBall& fireBall : mFireBalls)
		fireBall.Update(elapsedTime);

	auto it = std::remove_if(mFireBalls.begin(), mFireBalls.end(), IsFireBallDestroyed);
	mFireBalls.erase(it, mFireBalls.end());
}