#pragma once
#pragma once

#include "Common.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "GameObject.h"

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
	
	SharedTexture mAirHockeyTexture;
	SharedFont mAirHockeyTextFont;
	SharedSurface mAirHockeyTextSurface;
	SDL_Rect mAirHockeyTextRect;

	SharedTexture mEnterTexture;
	SharedFont mEnterTextFont;
	SharedSurface mEnterTextSurface;
	SDL_Rect mEnterTextRect;
};

