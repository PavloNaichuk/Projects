#include "Enemy.h"
#include "Config.h"

Enemy::Enemy(State state, const Point& center, const Vector& velocity, const Size& size)
	: GameUnit(state, center, velocity, size)
{
}

void Enemy::Update(float elapsedTime)
{
	if (mState == State::Running)
	{
		mCenter += mVelocity * elapsedTime;
		if ((mCenter.mX - mHalfSize.mX < 0) || (mCenter.mX + mHalfSize.mX > WINDOW_WIDTH))
			mVelocity.mX *= -1;
	}
}