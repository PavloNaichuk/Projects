#include "RenderEndMenu.h"

RenderEndMenu::RenderEndMenu(SDLRendererPointer& renderer, const int windowWidth, const int windowHeight)
	: mRenderer(renderer)
	, mTextColor({ 255, 255, 255, 255 })
	, mGameOverTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 130), TTF_CloseFont)
	, mGameOverTextSurface(TTF_RenderText_Solid(mGameOverTextFont.get(), "Game Over", mTextColor), SDL_FreeSurface)
	, mGameOverTextTexture(SDL_CreateTextureFromSurface(renderer.get(), mGameOverTextSurface.get()), SDL_DestroyTexture)
	, mGameOverTextRect()

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
		SDL_Log("Unable to create texture: %s", TTF_GetError());
	}
	SDL_QueryTexture(mGameOverTextTexture.get(), nullptr, nullptr, &mGameOverTextRect.w, &mGameOverTextRect.h);
	mGameOverTextRect.x = windowWidth / 2 - mGameOverTextRect.w / 2;
	mGameOverTextRect.y = windowHeight / 2 - mGameOverTextRect.h / 2 - 50;
}

void RenderEndMenu::render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mGameOverTextTexture.get(), nullptr, &mGameOverTextRect);
	SDL_RenderPresent(mRenderer.get());
}