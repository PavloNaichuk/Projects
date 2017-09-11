#include "FireBall.h"
#include "Config.h"

FireBall::FireBall(State state, const Point& center, const Vector& velocity, const Size& size)
	: GameUnit(state, center, velocity, size)
	, mCollidedWithGround(false)
{
}

void FireBall::Update(float elapsedTime)
{
	if (mState == State::Moving)
	{
		const float groundY = WINDOW_HEIGHT / 2 + 50;
		const Vector groundNormal(0.0f, -1.0f);

		mCenter += mVelocity * elapsedTime;
		mVelocity.mY += GRAVITY * elapsedTime;

		if (mCollidedWithGround) 
		{
			mVelocity = Reflect(mVelocity, groundNormal);
			mCollidedWithGround = false;
		}
		else if (mCenter.mY + mHalfSize.mY > groundY)
		{
			mCollidedWithGround = true;
			mCenter.mY = groundY - mHalfSize.mY;
		}
	}
}
