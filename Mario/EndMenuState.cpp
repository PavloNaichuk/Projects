#include "EndMenuState.h"

EndMenuState::EndMenuState(EndMenuStateListener& listener, SDLRendererPointer renderer, const int windowWidth, const int windowHeight)
	: mListener(listener)
	, mRenderer(renderer)
	, mTextColor({ 255, 255, 255, 255 })
	, mGameOverTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 130), TTF_CloseFont)
	, mGameOverTextSurface(TTF_RenderText_Solid(mGameOverTextFont.get(), "Game Over", mTextColor), SDL_FreeSurface)
	, mGameOverTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mGameOverTextSurface.get()), SDL_DestroyTexture)
{
	if (mGameOverTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
	}
	if (mGameOverTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
	}
	if (mGameOverTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	SDL_QueryTexture(mGameOverTextTexture.get(), nullptr, nullptr, &mGameOverTextRect.w, &mGameOverTextRect.h);
	mGameOverTextRect.x = (windowWidth - mGameOverTextRect.w) / 2;
	mGameOverTextRect.y = (windowHeight - mGameOverTextRect.h) / 2 - 50;
}

void EndMenuState::Enter()
{
}

void EndMenuState::Exit()
{	
}

void EndMenuState::ProcessKeyPressed(SDL_Keycode key)
{
}

void EndMenuState::ProcessKeyReleased(SDL_Keycode key)
{
}

void EndMenuState::Update(float elapsedTime)
{
}

void EndMenuState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mGameOverTextTexture.get(), nullptr, &mGameOverTextRect);
	SDL_RenderPresent(mRenderer.get());
}