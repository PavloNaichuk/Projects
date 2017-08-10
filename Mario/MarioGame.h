#pragma once
#include "StartMenuStateListener.h" 
#include "PlayGameStateListener.h"
#include "EndMenuStateListener.h"
#include "GameState.h"
#include "Common.h"

class MarioGame : public StartMenuStateListener, public PlayGameStateListener, public EndMenuStateListener
{
public:
	void LaunchGame();
	void EnterState(GameState* newState);
};
