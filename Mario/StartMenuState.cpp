#include "StartMenuState.h"

StartMenuState::StartMenuState(StartMenuStateListener& listener, SDLRendererPointer renderer, const int windowWidth, const int windowHeight)
	: mListener(listener)
	, mRenderer(renderer)
	, mTextColor({ 255, 255, 255, 255 })
	, mMarioTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 130), TTF_CloseFont)
	, mMarioTextSurface(TTF_RenderText_Solid(mMarioTextFont.get(), "Mario", mTextColor), SDL_FreeSurface)
	, mMarioTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioTextSurface.get()), SDL_DestroyTexture)
	, mEnterTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 30), TTF_CloseFont)
	, mEnterTextSurface(TTF_RenderText_Solid(mEnterTextFont.get(), "Press Space to enter", mTextColor), SDL_FreeSurface)
	, mEnterTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mEnterTextSurface.get()), SDL_DestroyTexture)
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
	mMarioTextRect.x = (windowWidth - mMarioTextRect.w) / 2;
	mMarioTextRect.y = (windowHeight - mMarioTextRect.h) / 2 - 50;

	if (mEnterTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}
	if (mEnterTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}
	if (mEnterTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", TTF_GetError());
	}

	SDL_QueryTexture(mEnterTextTexture.get(), nullptr, nullptr, &mEnterTextRect.w, &mEnterTextRect.h);
	mEnterTextRect.x = (windowWidth - mEnterTextRect.w) / 2;
	mEnterTextRect.y = (windowHeight - mEnterTextRect.h) / 2 + mEnterTextRect.h + 50;
}

void StartMenuState::Enter()
{
}

void StartMenuState::Exit()
{
}

void StartMenuState::ProcessKeyboard(SDL_Keycode key)
{
	if (key == SDLK_SPACE) 
	{
		mListener.OnGameStart();
	}
}

void StartMenuState::Update()
{
}

void StartMenuState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mMarioTextTexture.get(), nullptr, &mMarioTextRect);
	SDL_RenderCopy(mRenderer.get(), mEnterTextTexture.get(), nullptr, &mEnterTextRect);
	SDL_RenderPresent(mRenderer.get());
}