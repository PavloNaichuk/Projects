#include "pch.h"
#include "EndMenuState.h"
#include "Config.h"

EndMenuState::EndMenuState(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
	, mGameOverTextFont(resourceManager->GetTextFont(ResourceManager::GAME_OVER_ID))
{
}

void EndMenuState::Enter()
{
}

void EndMenuState::Update(float deltaTime)
{
}

void EndMenuState::Render()
{
	SDL_Color textColor = { 255, 255, 255 };

	char textBuffer[64];
	std::snprintf(textBuffer, sizeof(textBuffer), "Game Over");

	mGameOverTextSurface.reset(TTF_RenderText_Solid(mGameOverTextFont.get(), textBuffer, textColor), SDL_FreeSurface);
	assert(mGameOverTextSurface);

	mGameOverTextTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mGameOverTextSurface.get()), SDL_DestroyTexture);
	assert(mGameOverTextTexture);

	SDL_QueryTexture(mGameOverTextTexture.get(), nullptr, nullptr, &mGameOverTextRect.w, &mGameOverTextRect.h);

	mGameOverTextRect.x = (BOARD_WIDTH - mGameOverTextRect.w) / 2;
	mGameOverTextRect.y = (BOARD_HEIGHT - mGameOverTextRect.h) / 2 - 50;

	SDL_SetRenderDrawColor(mRenderer.get(), 0, 255, 0, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mGameOverTextTexture.get(), nullptr, &mGameOverTextRect);
	SDL_RenderPresent(mRenderer.get());
}

void EndMenuState::Exit()
{
}