#pragma once

#include "Common.h"
#include "GameState.h"
#include "ResourceManager.h"

class AirHockey
{
public:
	int LaunchGame();

private:
	int Init();
	int Deinit();
	void GameLoop();

	void EnterState(std::unique_ptr<GameState> newState);

private:
	SharedWindow mWindow;
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;
	std::unique_ptr<GameState> mCurrentState;
};
