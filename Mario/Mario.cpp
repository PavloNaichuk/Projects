#include "Mario.h"
#include "Config.h"

Mario::Mario(State state, const Point& center, const Vector& velocity, const Size& size)
	: GameUnit(state, center, velocity, size)
{
}

void Mario::ProcessKeyPressed(SDL_Keycode key)
{
	if (mState == State::Standing)
	{
		if (key == SDLK_LEFT)
		{
			mState = State::Running;
			mVelocity = Vector(-MOVE_SPEED, 0);
		}
		else if (key == SDLK_RIGHT)
		{
			mState = State::Running;
			mVelocity = Vector(MOVE_SPEED, 0);
		}
		else if (key == SDLK_DOWN)
		{
			mState = State::Running;
			mVelocity = Vector(0, MOVE_SPEED);
		}
		else if (key == SDLK_UP)
		{
			mState = State::Running;
			mVelocity = Vector(0, -MOVE_SPEED);
		}
		else if (key == SDLK_SPACE)
		{
			mState = State::Jumping;
			mVelocity.mY = -JUMP_SPEED;
		}
	}
}

void Mario::ProcessKeyReleased(SDL_Keycode key)
{
	if (mState == State::Running)
	{
		if ((key == SDLK_LEFT) || (key == SDLK_RIGHT) || (key == SDLK_DOWN) || (key == SDLK_UP))
		{
			mState = State::Standing;
			mVelocity = Vector(0, 0);
		}
	}
}

void Mario::Update(float elapsedTime)
{
	if (mState == State::Jumping)
	{
		mCenter += mVelocity * elapsedTime;
		mVelocity.mY += GRAVITY * elapsedTime;

		if (mCenter.mY + mHalfSize.mY > WINDOW_HEIGHT)
		{
			mCenter.mY = WINDOW_HEIGHT - mHalfSize.mY;
			mState = State::Standing;
		}
	}
	else if (mState == State::Running)
	{
		mCenter += mVelocity * elapsedTime;

		/*if (mGameUnit.mCenter.mX - mGameUnit.mHalfSize.mX < 0)
		mGameUnit.mCenter.mX = mGameUnit.mHalfSize.mX;

		if (mGameUnit.mCenter.mX + mGameUnit.mHalfSize.mX > WINDOW_WIDTH)
		mGameUnit.mCenter.mX = WINDOW_WIDTH - mGameUnit.mHalfSize.mX;

		if (mGameUnit.mCenter.mY + mGameUnit.mHalfSize.mY > WINDOW_HEIGHT)
		mGameUnit.mCenter.mY = WINDOW_HEIGHT - mGameUnit.mHalfSize.mY;

		if (mGameUnit.mCenter.mY - mGameUnit.mHalfSize.mY < 0)
		mGameUnit.mCenter.mY = mGameUnit.mHalfSize.mY;*/
	}

}
