#include "PlayGameState.h"

PlayGameState::PlayGameState(PlayGameStateListener& listener, SDLRendererPointer renderer)
	: mListener(listener)
	, mRenderer(renderer)
	, mTextColor({ 255, 255, 255, 255 })
	, mMarioTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mMarioTextSurface(TTF_RenderText_Solid(mMarioTextFont.get(), "Mario: 0", mTextColor), SDL_FreeSurface)
	, mMarioTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioTextSurface.get()), SDL_DestroyTexture)
	, mTimeTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mTimeTextSurface(TTF_RenderText_Solid(mTimeTextFont.get(), "Timer: 0", mTextColor), SDL_FreeSurface)
	, mTimeTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mTimeTextSurface.get()), SDL_DestroyTexture)
	, mImageSurface(IMG_Load("Resources/Images/mario.JPG"), SDL_FreeSurface)
	, mImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mImageSurface.get()), SDL_DestroyTexture)
	, mGameUnit(State::Standing, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(MARIO_WIDTH, MARIO_HEIGHT))
{
	if (mMarioTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}
	if (mMarioTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}
	if (mMarioTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_QueryTexture(mMarioTextTexture.get(), nullptr, nullptr, &mMarioTextRect.w, &mMarioTextRect.h);
	mMarioTextRect.x = 10;
	mMarioTextRect.y = 10;

	if (mTimeTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}
	if (mTimeTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}
	if (mTimeTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_QueryTexture(mTimeTextTexture.get(), nullptr, nullptr, &mTimeTextRect.w, &mTimeTextRect.h);
	mTimeTextRect.x = 1000;
	mTimeTextRect.y = 10;
	
	if (mImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mImageTexture == nullptr) 
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_Rect destRect;
	SDL_QueryTexture(mImageTexture.get(), nullptr, nullptr, &destRect.w, &destRect.h);
	destRect.x = 10;
	destRect.y = 10;

	mGameUnit.mCenter.mX = WINDOW_WIDTH / 2;
	mGameUnit.mCenter.mY= WINDOW_HEIGHT / 2;
}

PlayGameState::~PlayGameState() 
{
}

void PlayGameState::Enter()
{
}

void PlayGameState::Exit()
{
}

void PlayGameState::ProcessKeyPressed(SDL_Keycode key)
{
	float elapsedTime = 1.0f;
	if (key == SDLK_LEFT)
	{
		mGameUnit.mState = State::Running;
		mGameUnit.mVelocity = Vector(-MOVE_SPEED, 0);
	}
	else if (key == SDLK_RIGHT)
	{
		mGameUnit.mState = State::Running;
		mGameUnit.mVelocity = Vector(MOVE_SPEED, 0);
	}
	else if (key == SDLK_DOWN)
	{
		mGameUnit.mState = State::Running;
		mGameUnit.mVelocity = Vector(0, MOVE_SPEED);
	}
	else if (key == SDLK_UP)
	{
		mGameUnit.mState = State::Running;
		mGameUnit.mVelocity = Vector(0, -MOVE_SPEED);
	}
	else if (key == SDLK_SPACE)
	{
		if (mGameUnit.mState != State::Jumping) 
		{
			mGameUnit.mState = State::Jumping;
			mGameUnit.mVelocity.mY = -JUMP_SPEED;
		}
	}
}

void PlayGameState::ProcessKeyReleased(SDL_Keycode key)
{
	if ((key == SDLK_LEFT) || (key == SDLK_RIGHT) || (key == SDLK_DOWN) || (key == SDLK_UP))
	{
		if (mGameUnit.mState == State::Running) 
		{
			mGameUnit.mState = State::Standing;
			mGameUnit.mVelocity = Vector(0, 0);
		}
	}
}

void PlayGameState::Update(float elapsedTime)
{
	if (mGameUnit.mState == State::Jumping) 
	{
		mGameUnit.mCenter += mGameUnit.mVelocity * elapsedTime;
		mGameUnit.mVelocity.mY += GRAVITY * elapsedTime;

		if (mGameUnit.mCenter.mY + mGameUnit.mHalfSize.mY > WINDOW_HEIGHT) 
		{
			mGameUnit.mCenter.mY = WINDOW_HEIGHT - mGameUnit.mHalfSize.mY;
			mGameUnit.mState = State::Standing;
		}
	}
	else if (mGameUnit.mState == State::Running)
	{
		mGameUnit.mCenter += mGameUnit.mVelocity * elapsedTime;

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

void PlayGameState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mMarioTextTexture.get(), nullptr, &mMarioTextRect);
	SDL_RenderCopy(mRenderer.get(), mTimeTextTexture.get(), nullptr, &mTimeTextRect);

	Size size = 2.0f * mGameUnit.mHalfSize;
	Point topLeft = mGameUnit.mCenter - mGameUnit.mHalfSize;
	SDL_Rect destRect = {(int)topLeft.mX, (int)topLeft.mY, (int)size.mX, (int)size.mY};

	SDL_RenderCopy(mRenderer.get(), mImageTexture.get(), nullptr, &destRect);
	SDL_RenderPresent(mRenderer.get());
}

