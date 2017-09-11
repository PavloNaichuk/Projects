#include "Mario.h"
#include "Config.h"
#include "GameWorld.h"

Mario::Mario(State state, const Point& center, const Vector& velocity, const Size& size)
	: GameUnit(state, center, velocity, size)
{
}

void Mario::ProcessKeyPressed(SDL_Keycode key, GameWorld& gameWorld)
{
	if (gameWorld.mMario.mState == State::Standing)
	{
		if (key == SDLK_LEFT)
		{
			gameWorld.mMario.mState = State::Moving;
			gameWorld.mMario.mVelocity = Vector(-MOVE_SPEED, 0);
		}
		else if (key == SDLK_RIGHT)
		{
			gameWorld.mMario.mState = State::Moving;
			gameWorld.mMario.mVelocity = Vector(MOVE_SPEED, 0);
		}
		else if (key == SDLK_DOWN)
		{
			gameWorld.mMario.mState = State::Moving;
			gameWorld.mMario.mVelocity = Vector(0, MOVE_SPEED);
		}
		else if (key == SDLK_UP)
		{
			gameWorld.mMario.mState = State::Moving;
			gameWorld.mMario.mVelocity = Vector(0, -MOVE_SPEED);
		}
		else if (key == SDLK_SPACE)
		{
			gameWorld.mMario.mState = State::Jumping;
			gameWorld.mMario.mVelocity.mY = -JUMP_SPEED;
		}
		else if (key == SDLK_LCTRL)
		{
			gameWorld.mFireBalls.emplace_back(State::Moving, mCenter,
				Vector(-MOVE_SPEED, 0.0f),
				Size(FAREBALL_WIDTH, FAREBALL_HEIGHT));
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
		mVelocity.mY += GRAVITY * elapsedTime;

		if (mCenter.mY + mHalfSize.mY > WINDOW_HEIGHT)
		{
			mCenter.mY = WINDOW_HEIGHT - mHalfSize.mY;
			mState = State::Standing;
		}
	}
	else if (mState == State::Moving)
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
