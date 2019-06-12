#include "pch.h"
#include "StartMenuState.h"
#include "Config.h"
#include "RenderComponent.h"

StartMenuState::StartMenuState(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
	, mTextColor({ 255, 255, 255, 255 })
	, mAirHockeyTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 130), TTF_CloseFont)
	, mAirHockeyTextSurface(TTF_RenderText_Solid(mAirHockeyTextFont.get(), "AirHockey", mTextColor), SDL_FreeSurface)
	, mAirHockeyTexture(SDL_CreateTextureFromSurface(renderer.get(), mAirHockeyTextSurface.get()), SDL_DestroyTexture)
	, mEnterTextFont(TTF_OpenFont("Resources/Fonts/Arial.TTF", 30), TTF_CloseFont)
	, mEnterTextSurface(TTF_RenderText_Solid(mEnterTextFont.get(), "Press Space to enter", mTextColor), SDL_FreeSurface)
	, mEnterTexture(SDL_CreateTextureFromSurface(renderer.get(), mEnterTextSurface.get()), SDL_DestroyTexture)
{

	SDL_QueryTexture(mAirHockeyTexture.get(), nullptr, nullptr, &mAirHockeyTextRect.w, &mAirHockeyTextRect.h);
	mAirHockeyTextRect.x = (BOARD_WIDTH - mAirHockeyTextRect.w) / 2;
	mAirHockeyTextRect.y = (BOARD_HEIGHT - mAirHockeyTextRect.h) / 2 - 50;

	SDL_QueryTexture(mEnterTexture.get(), nullptr, nullptr, &mEnterTextRect.w, &mEnterTextRect.h);
	mEnterTextRect.x = (BOARD_WIDTH - mEnterTextRect.w) / 2;
	mEnterTextRect.y = (BOARD_HEIGHT - mEnterTextRect.h) / 2 + mEnterTextRect.h + 50;
}

void StartMenuState::Enter()
{
}

void StartMenuState::Exit()
{
}

void StartMenuState::Update(float elapsedTime)
{
}

void StartMenuState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mAirHockeyTexture.get(), nullptr, &mAirHockeyTextRect);
	SDL_RenderCopy(mRenderer.get(), mEnterTexture.get(), nullptr, &mEnterTextRect);
	SDL_RenderPresent(mRenderer.get());
}