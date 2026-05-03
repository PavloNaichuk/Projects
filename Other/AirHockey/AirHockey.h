#pragma once

#include "Common.h"
#include "GameState.h"
#include "ResourceManager.h"

class AirHockey
{
public:
	int LaunchGame();

private:
	using UniqueGameState = std::unique_ptr<GameState>;

	int Init();
	int Deinit();
	void GameLoop();

	void EnterState(UniqueGameState newState);

private:
	SharedWindow mWindow;
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;

	UniqueGameState mCurrentState;
};
