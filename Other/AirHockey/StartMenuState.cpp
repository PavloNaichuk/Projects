#include "pch.h"
#include "StartMenuState.h"
#include "Config.h"
#include "EventCenter.h"

StartMenuState::StartMenuState(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
	, mAirHockeyTextFont(resourceManager->GetTextFont(ResourceManager::GAME_TITLE_TEXT_ID))
	, mNavigationTextFont(resourceManager->GetTextFont(ResourceManager::NAVIGATION_TEXT_ID))
	, mStartGameTextFont(resourceManager->GetTextFont(ResourceManager::START_GAME_TEXT_ID))
{
}

void StartMenuState::Enter()
{
	SDL_Color textColor = { 255, 255, 255 };

	const char* airHockeyText =  "Air Hockey";

	mAirHockeyTextSurface.reset(TTF_RenderText_Solid(mAirHockeyTextFont.get(), airHockeyText, textColor), SDL_FreeSurface);
	assert(mAirHockeyTextSurface);
	mAirHockeyTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mAirHockeyTextSurface.get()), SDL_DestroyTexture);
	assert(mAirHockeyTexture);

	SDL_QueryTexture(mAirHockeyTexture.get(), nullptr, nullptr, &mAirHockeyTextRect.w, &mAirHockeyTextRect.h);
	mAirHockeyTextRect.x = (BOARD_WIDTH - mAirHockeyTextRect.w) / 2;
	mAirHockeyTextRect.y = mAirHockeyTextRect.h + 20;
	
	const char* navigationText = "Left, Right, Down, Up - navigate, Ctrl - kick the puck";

	mNavigationTextSurface.reset(TTF_RenderText_Solid(mNavigationTextFont.get(), navigationText, textColor), SDL_FreeSurface);
	assert(mNavigationTextSurface);
	mNavigationTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mNavigationTextSurface.get()), SDL_DestroyTexture);
	assert(mNavigationTexture);

	SDL_QueryTexture(mNavigationTexture.get(), nullptr, nullptr, &mNavigationTextRect.w, &mNavigationTextRect.h);
	mNavigationTextRect.x = (BOARD_WIDTH - mNavigationTextRect.w) / 2;
	mNavigationTextRect.y = mAirHockeyTextRect.y + mNavigationTextRect.h + 60;

	const char* startGameText = "Press Space to start";

	mStartGameTextSurface.reset(TTF_RenderText_Solid(mStartGameTextFont.get(), startGameText, textColor), SDL_FreeSurface);
	assert(mStartGameTextSurface);
	mStartGameTexture.reset(SDL_CreateTextureFromSurface(mRenderer.get(), mStartGameTextSurface.get()), SDL_DestroyTexture);
	assert(mStartGameTexture);

	SDL_QueryTexture(mStartGameTexture.get(), nullptr, nullptr, &mStartGameTextRect.w, &mStartGameTextRect.h);
	mStartGameTextRect.x = (BOARD_WIDTH - mStartGameTextRect.w) / 2;
	mStartGameTextRect.y = mNavigationTextRect.y + mStartGameTextRect.h  + 60;
}

void StartMenuState::Exit()
{
}

void StartMenuState::Update(float elapsedTime)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	if (keyboardState[SDL_SCANCODE_SPACE] == 1)
	{
		EventCenter::GetInstance().Send(std::make_unique<Event>(Event::START_GAME_ID, 0));
	}
}

void StartMenuState::Render()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 0);
	SDL_RenderClear(mRenderer.get());

	SDL_RenderCopy(mRenderer.get(), mAirHockeyTexture.get(), nullptr, &mAirHockeyTextRect);
	SDL_RenderCopy(mRenderer.get(), mNavigationTexture.get(), nullptr, &mNavigationTextRect);
	SDL_RenderCopy(mRenderer.get(), mStartGameTexture.get(), nullptr, &mStartGameTextRect);

	SDL_RenderPresent(mRenderer.get());
}