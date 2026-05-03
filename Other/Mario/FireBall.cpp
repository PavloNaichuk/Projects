#include "FireBall.h"
#include "Config.h"
#include "GameWorld.h"
#include "Collision.h"

FireBall::FireBall(State state, const Point& center, const Vector& velocity, const Size& size)
	: GameUnit(state, center, velocity, size)
	, mIsActive(true)
	, mCollidedWithGround(false)
{
}

void FireBall::Update(float elapsedTime, GameWorld& gameWorld)
{
	if (mState == State::Moving)
	{
		const float groundY = WINDOW_HEIGHT / 2 + 50;
		const Vector groundNormal(0.0f, -1.0f);

		mCenter += mVelocity * elapsedTime;
		mVelocity.mY += FIREBALL_GRAVITY * elapsedTime;

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
		
		TestCollisionVersusEnemy(gameWorld);
	}
}

void FireBall::TestCollisionVersusEnemy(GameWorld& gameWorld)
{
	for (Enemy& enemy : gameWorld.mEnemies)
	{
		if (Collide(*this, enemy))
		{
			gameWorld.mMario.mScores += enemy.mScores;
			enemy.mIsActive = false;
			mIsActive = false;

			break;
		}
	}
}
