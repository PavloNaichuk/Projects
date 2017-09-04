#include "PlayGameState.h"

PlayGameState::PlayGameState(PlayGameStateListener& listener, SDLRendererPointer renderer)
	: mListener(listener)
	, mTextColor({ 255, 255, 255, 255 })
	, mMarioTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mMarioTextSurface(TTF_RenderText_Solid(mMarioTextFont.get(), "Mario: 0", mTextColor), SDL_FreeSurface)
	, mMarioTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioTextSurface.get()), SDL_DestroyTexture)
	, mTimeTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mTimeTextSurface(TTF_RenderText_Solid(mTimeTextFont.get(), "Timer: 0", mTextColor), SDL_FreeSurface)
	, mTimeTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mTimeTextSurface.get()), SDL_DestroyTexture)
	, mMario(State::Standing, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(MARIO_WIDTH, MARIO_HEIGHT))
	, mEnemy(State::Running, Point(0.0f, 0.0f), Vector(0.0f, 0.0f), Size(ENEMY_WIDTH, ENEMY_HEIGHT))
	, mGameRenderer(renderer)
{
	mMario.mCenter.mX = WINDOW_WIDTH / 2;
	mMario.mCenter.mY = WINDOW_HEIGHT / 2;

	mEnemy.mVelocity.mX = -MOVE_SPEED;
	mEnemy.mCenter.mX = WINDOW_WIDTH / 2;
	mEnemy.mCenter.mY = WINDOW_HEIGHT - mEnemy.mHalfSize.mY;

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
	mMario.ProcessKeyPressed(key);
}

void PlayGameState::ProcessKeyReleased(SDL_Keycode key)
{
	mMario.ProcessKeyReleased(key);
}

void PlayGameState::Update(float elapsedTime)
{
	mMario.Update(elapsedTime);
	mEnemy.Update(elapsedTime);
}

void PlayGameState::Render()
{
	mGameRenderer.BeginRenderFrame();

	/*SDL_RenderCopy(mRenderer.get(), mMarioTextTexture.get(), nullptr, &mMarioTextRect);
	SDL_RenderCopy(mRenderer.get(), mTimeTextTexture.get(), nullptr, &mTimeTextRect);*/
	
	mGameRenderer.Render(mMario);
	mGameRenderer.Render(mEnemy);
	
	mGameRenderer.EndRenderFrame();
}

