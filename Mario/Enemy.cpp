#include "Enemy.h"
#include "Config.h"
#include "GameWorld.h"
#include "Collision.h"

Enemy::Enemy(State state, const Point& center, const Vector& velocity, const Size& size, int scores)
	: GameUnit(state, center, velocity, size)
	, mScores(scores)
	, mIsActive(true)
{
}

void Enemy::Update(float elapsedTime, GameWorld& gameWorld)
{
	if (!mIsActive)
		return;

	if (mState == State::Moving)
	{
		mCenter += mVelocity * elapsedTime;
		if ((mCenter.mX - mHalfSize.mX < 0) || (mCenter.mX + mHalfSize.mX > WINDOW_WIDTH))
			mVelocity.mX *= -1;
	}

	if (Collide(*this, gameWorld.mMario))
		gameWorld.mMario.RemoveLife();	
}