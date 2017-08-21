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
		SDL_Log("Unable to create texture: %s", TTF_GetError());
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
		SDL_Log("Unable to create texture: %s", TTF_GetError());
	}

	SDL_QueryTexture(mTimeTextTexture.get(), nullptr, nullptr, &mTimeTextRect.w, &mTimeTextRect.h);
	mTimeTextRect.x = 1000;
	mTimeTextRect.y = 10;
}

PlayGameState::~PlayGameState() 
{}

void PlayGameState::Enter()
{
}

void PlayGameState::Exit()
{
}

void PlayGameState::ProcessKeyboard(SDL_Keycode key)
{
}

void PlayGameState::Update()
{
}

void PlayGameState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mMarioTextTexture.get(), nullptr, &mMarioTextRect);
	SDL_RenderCopy(mRenderer.get(), mTimeTextTexture.get(), nullptr, &mTimeTextRect);
	SDL_RenderPresent(mRenderer.get());
}

