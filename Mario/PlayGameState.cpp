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
	, mMarioImageSurface(IMG_Load("Resources/Images/mario.JPG"), SDL_FreeSurface)
	, mMarioImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioImageSurface.get()), SDL_DestroyTexture)
	, mMario(State::Standing, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(MARIO_WIDTH, MARIO_HEIGHT))
	, mEnemy(State::Running, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(MARIO_WIDTH, MARIO_HEIGHT))
	, mEnemyImageSurface(IMG_Load("Resources/Images/owl.JPG"), SDL_FreeSurface)
	, mEnemyImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mEnemyImageSurface.get()), SDL_DestroyTexture)
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
	
	if (mMarioImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mMarioImageTexture == nullptr) 
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_Rect destRect;
	SDL_QueryTexture(mMarioImageTexture.get(), nullptr, nullptr, &destRect.w, &destRect.h);
	destRect.x = 10;
	destRect.y = 10;

	mMario.mCenter.mX = WINDOW_WIDTH / 2;
	mMario.mCenter.mY= WINDOW_HEIGHT / 2;


	if (mEnemyImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mEnemyImageTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_Rect destEnemyRect;
	SDL_QueryTexture(mEnemyImageTexture.get(), nullptr, nullptr, &destEnemyRect.w, &destEnemyRect.h);
	destEnemyRect.x = 10;
	destEnemyRect.y = 10;
	mEnemy.mVelocity.mX = -MOVE_SPEED;
	mEnemy.mCenter.mX = WINDOW_WIDTH / 2;
	mEnemy.mCenter.mY = WINDOW_HEIGHT - mEnemy.mHalfSize.mY;
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
	if (mMario.mState == State::Standing) 
	{
		if (key == SDLK_LEFT)
		{
			mMario.mState = State::Running;
			mMario.mVelocity = Vector(-MOVE_SPEED, 0);
		}
		else if (key == SDLK_RIGHT)
		{
			mMario.mState = State::Running;
			mMario.mVelocity = Vector(MOVE_SPEED, 0);
		}
		else if (key == SDLK_DOWN)
		{
			mMario.mState = State::Running;
			mMario.mVelocity = Vector(0, MOVE_SPEED);
		}
		else if (key == SDLK_UP)
		{
			mMario.mState = State::Running;
			mMario.mVelocity = Vector(0, -MOVE_SPEED);
		}
		else if (key == SDLK_SPACE)
		{
			mMario.mState = State::Jumping;
			mMario.mVelocity.mY = -JUMP_SPEED;
		}
	}
}

void PlayGameState::ProcessKeyReleased(SDL_Keycode key)
{
	if (mMario.mState == State::Running)
	{
		if ((key == SDLK_LEFT) || (key == SDLK_RIGHT) || (key == SDLK_DOWN) || (key == SDLK_UP))
		{
			mMario.mState = State::Standing;
			mMario.mVelocity = Vector(0, 0);
		}
	}
}

void PlayGameState::Update(float elapsedTime)
{
	if (mMario.mState == State::Jumping) 
	{
		mMario.mCenter += mMario.mVelocity * elapsedTime;
		mMario.mVelocity.mY += GRAVITY * elapsedTime;

		if (mMario.mCenter.mY + mMario.mHalfSize.mY > WINDOW_HEIGHT) 
		{
			mMario.mCenter.mY = WINDOW_HEIGHT - mMario.mHalfSize.mY;
			mMario.mState = State::Standing;
		}
	}
	else if (mMario.mState == State::Running)
	{
		mMario.mCenter += mMario.mVelocity * elapsedTime;

		/*if (mGameUnit.mCenter.mX - mGameUnit.mHalfSize.mX < 0)
			mGameUnit.mCenter.mX = mGameUnit.mHalfSize.mX;

		if (mGameUnit.mCenter.mX + mGameUnit.mHalfSize.mX > WINDOW_WIDTH)
			mGameUnit.mCenter.mX = WINDOW_WIDTH - mGameUnit.mHalfSize.mX;
			
		if (mGameUnit.mCenter.mY + mGameUnit.mHalfSize.mY > WINDOW_HEIGHT)
			mGameUnit.mCenter.mY = WINDOW_HEIGHT - mGameUnit.mHalfSize.mY;

		if (mGameUnit.mCenter.mY - mGameUnit.mHalfSize.mY < 0)
			mGameUnit.mCenter.mY = mGameUnit.mHalfSize.mY;*/
	}

	if (mEnemy.mState == State::Running) 
	{
		mEnemy.mCenter += mEnemy.mVelocity * elapsedTime;
		if ((mEnemy.mCenter.mX - mEnemy.mHalfSize.mX < 0) || (mEnemy.mCenter.mX + mEnemy.mHalfSize.mX > WINDOW_WIDTH))
			mEnemy.mVelocity.mX *= -1;
	}
}

void PlayGameState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mMarioTextTexture.get(), nullptr, &mMarioTextRect);
	SDL_RenderCopy(mRenderer.get(), mTimeTextTexture.get(), nullptr, &mTimeTextRect);

	Size size = 2.0f * mMario.mHalfSize;
	Point topLeft = mMario.mCenter - mMario.mHalfSize;
	SDL_Rect destRect = {(int)topLeft.mX, (int)topLeft.mY, (int)size.mX, (int)size.mY};
	SDL_RenderCopy(mRenderer.get(), mMarioImageTexture.get(), nullptr, &destRect);

	Size sizeEnemy = 2.0f * mEnemy.mHalfSize;
	Point topLeftEnemy = mEnemy.mCenter - mEnemy.mHalfSize;
	SDL_Rect destEnemyRect = { (int)topLeftEnemy.mX, (int)topLeftEnemy.mY, (int)sizeEnemy.mX, (int)sizeEnemy.mY };
	SDL_RenderCopy(mRenderer.get(), mEnemyImageTexture.get(), nullptr, &destEnemyRect);

	SDL_RenderPresent(mRenderer.get());
}

