#pragma once
#pragma once

#include "Common.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "EventCenter.h"

class StartMenuStateListener
{
public:
	virtual ~StartMenuStateListener() {}
	virtual void OnGameStart() = 0;
};

class StartMenuState : public GameState
{
public:
	StartMenuState(SharedRenderer renderer, SharedResourceManager resourceManager);

	void Enter() override;
	void Exit() override;
	void Update(float elapsedTime) override;
	void Render() override;

private:
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;
	
	SharedFont mAirHockeyTextFont;
	SharedSurface mAirHockeyTextSurface;
	SharedTexture mAirHockeyTexture;
	SDL_Rect mAirHockeyTextRect;

	SharedFont mNavigationTextFont;
	SharedSurface mNavigationTextSurface;
	SharedTexture mNavigationTexture;
	SDL_Rect mNavigationTextRect;

	SharedFont mStartGameTextFont;
	SharedSurface mStartGameTextSurface;
	SharedTexture mStartGameTexture;
	SDL_Rect mStartGameTextRect;
};

