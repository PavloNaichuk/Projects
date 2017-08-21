#pragma once

#include "Common.h"
#include "StartMenuState.h" 
#include "PlayGameState.h"
#include "EndMenuState.h"

class MarioGame : public StartMenuStateListener, public PlayGameStateListener, public EndMenuStateListener
{
public:
	MarioGame();
	~MarioGame();

	void LaunchGame();

private:
	void EnterState(std::unique_ptr<GameState> newState);

	void OnGameStart() override;
	void OnLevelLose() override;
	void OnLevelComplete() override;
	void OnGameOver() override;

private:
	SDLWindowPointer mWindow;
	SDLRendererPointer mRenderer;
	std::unique_ptr<GameState> mCurrentState;
};
