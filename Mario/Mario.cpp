#include "Mario.h"
#include "Config.h"
#include "GameWorld.h"

Mario::Mario(State state, const Point& center, const Vector& velocity, const Size& size)
	: GameUnit(state, center, velocity, size)
	, mDirection(1.0f)
{
}

void Mario::ProcessKeyPressed(SDL_Keycode key, GameWorld& gameWorld)
{
	if (mState == State::Standing)
	{
		if (key == SDLK_LEFT)
		{
			mState = State::Moving;
			mVelocity = Vector(-MARIO_SPEED, 0);
			mDirection = -1.0f;
		}
		else if (key == SDLK_RIGHT)
		{
			mState = State::Moving;
			mVelocity = Vector(MARIO_SPEED, 0);
			mDirection = 1.0f;
		}
		else if (key == SDLK_DOWN)
		{
			mState = State::Moving;
			mVelocity = Vector(0, MARIO_SPEED);
		}
		else if (key == SDLK_UP)
		{
			mState = State::Moving;
			mVelocity = Vector(0, -MARIO_SPEED);
		}
		else if (key == SDLK_SPACE)
		{
			mState = State::Jumping;
			mVelocity.mY = -MARIO_JUMP_SPEED;
		}
	}
	if ((mState == State::Standing) || (mState == State::Moving) || (mState == State::Jumping)) 
	{
		if (key == SDLK_LCTRL)
		{
			gameWorld.mFireBalls.emplace_back(State::Moving, mCenter,
				mDirection * Vector(FIREBALL_SPEED, 0),
				Size(FIREBALL_WIDTH, FIREBALL_HEIGHT));
		}
	}
}

void Mario::ProcessKeyReleased(SDL_Keycode key)
{
	if (mState == State::Moving)
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
		mVelocity.mY += MARIO_GRAVITY * elapsedTime;

		if (mCenter.mY + mHalfSize.mY > WINDOW_HEIGHT)
		{
			mCenter.mY = WINDOW_HEIGHT - mHalfSize.mY;
			mState = State::Standing;
		}
	}
	else if (mState == State::Moving)
	{
		mCenter += mVelocity * elapsedTime;

		if (mCenter.mX - mHalfSize.mX < 0)
			mCenter.mX = mHalfSize.mX;

		if (mCenter.mX + mHalfSize.mX > WINDOW_WIDTH)
			mCenter.mX = WINDOW_WIDTH - mHalfSize.mX;

		if (mCenter.mY + mHalfSize.mY > WINDOW_HEIGHT)
			mCenter.mY = WINDOW_HEIGHT - mHalfSize.mY;

		if (mCenter.mY - mHalfSize.mY < 0)
			mCenter.mY = mHalfSize.mY;
	}
}
