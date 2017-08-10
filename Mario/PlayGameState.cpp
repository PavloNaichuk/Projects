#include "PlayGameState.h"

PlayGameState::PlayGameState(SDLRendererPointer& renderer)
	: mRenderer(renderer)
	, mTextColor({ 255, 255, 255, 255 })
	, mMarTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mMarTextSurface(TTF_RenderText_Solid(mMarTextFont.get(), "Mario: 0", mTextColor), SDL_FreeSurface)
	, mMarTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarTextSurface.get()), SDL_DestroyTexture)
	, mMarTextRect()
	, mTimeTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 50), TTF_CloseFont)
	, mTimeTextSurface(TTF_RenderText_Solid(mTimeTextFont.get(), "Timer: 0", mTextColor), SDL_FreeSurface)
	, mTimeTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mTimeTextSurface.get()), SDL_DestroyTexture)
	, mTimeTextRect()
{
	if (mMarTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}


	if (mMarTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}


	if (mMarTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", TTF_GetError());
	}
	SDL_QueryTexture(mMarTextTexture.get(), nullptr, nullptr, &mMarTextRect.w, &mMarTextRect.h);
	mMarTextRect.x = 10;
	mMarTextRect.y = 10;

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
	SDL_RenderCopy(mRenderer.get(), mMarTextTexture.get(), nullptr, &mMarTextRect);
	SDL_RenderCopy(mRenderer.get(), mTimeTextTexture.get(), nullptr, &mTimeTextRect);
	SDL_RenderPresent(mRenderer.get());
}

