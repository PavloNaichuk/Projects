#include "pch.h"
#include "StartMenuState.h"
#include "Config.h"
#include "RenderComponent.h"

StartMenuState::StartMenuState(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
	, mAirHockeyTextFont(resourceManager->GetTextFont(ResourceManager::START_ID))
	, mEnterTextFont(resourceManager->GetTextFont(ResourceManager::ENTER_ID))
{

}

void StartMenuState::Enter()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
}

void StartMenuState::Exit()
{
}

void StartMenuState::ProcessKeyPressed(Event& event)
{

	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	if (keyboardState[SDL_SCANCODE_SPACE] == 1) 
	{
		event.mSenderId = Event::START_GAME_ID;
	}
	
}
void StartMenuState::Update(float elapsedTime)
{
}

void StartMenuState::Render()
{
	SDL_Color textColor = { 255, 255, 255 };

	char textBufferAirHockey[16];
	std::snprintf(textBufferAirHockey, sizeof(textBufferAirHockey), "AirHockey");

	char textBufferEnter[64];
	std::snprintf(textBufferEnter, sizeof(textBufferEnter), "Press Space to enter");

	mAirHockeyTextSurface.reset(TTF_RenderText_Solid(mAirHockeyTextFont.get(), textBufferAirHockey, textColor), SDL_FreeSurface);
	assert(mAirHockeyTextSurface);
	mEnterTextSurface.reset(TTF_RenderText_Solid(mEnterTextFont.get(), textBufferEnter, textColor), SDL_FreeSurface);
	assert(mEnterTextSurface);

	mAirHockeyTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mAirHockeyTextSurface.get()), SDL_DestroyTexture);
	assert(mAirHockeyTexture);
	mEnterTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mEnterTextSurface.get()), SDL_DestroyTexture);
	assert(mEnterTexture);

	SDL_QueryTexture(mAirHockeyTexture.get(), nullptr, nullptr, &mAirHockeyTextRect.w, &mAirHockeyTextRect.h);
	mAirHockeyTextRect.x = (BOARD_WIDTH - mAirHockeyTextRect.w) / 2;
	mAirHockeyTextRect.y = (BOARD_HEIGHT - mAirHockeyTextRect.h) / 2 - 50;

	SDL_QueryTexture(mEnterTexture.get(), nullptr, nullptr, &mEnterTextRect.w, &mEnterTextRect.h);
	mEnterTextRect.x = (BOARD_WIDTH - mEnterTextRect.w) / 2;
	mEnterTextRect.y = (BOARD_HEIGHT - mEnterTextRect.h) / 2 + mEnterTextRect.h + 50;

	SDL_SetRenderDrawColor(mRenderer.get(), 0, 255, 0, 0);
	SDL_RenderClear(mRenderer.get());
	SDL_RenderCopy(mRenderer.get(), mAirHockeyTexture.get(), nullptr, &mAirHockeyTextRect);
	SDL_RenderCopy(mRenderer.get(), mEnterTexture.get(), nullptr, &mEnterTextRect);

	SDL_RenderPresent(mRenderer.get());
}