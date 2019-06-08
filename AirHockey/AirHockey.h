#pragma once

#include "Common.h"
#include "GameState.h"
#include "ResourceManager.h"

class AirHockey
{
public:
	AirHockey();
	~AirHockey();

	void LaunchGame();

private:
	void EnterState(std::unique_ptr<GameState> newState);

private:
	UniqueWindowPointer mWindow;
	SharedRendererPointer mRenderer;
	SharedResourceManagerPointer mResourceManager;
	std::unique_ptr<GameState> mCurrentState;
};
