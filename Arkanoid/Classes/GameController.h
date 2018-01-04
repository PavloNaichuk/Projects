#pragma once

class GameController
{
public:
	virtual ~GameController() = default;

	virtual void startGame() = 0;
	virtual void playGame() = 0;
	virtual void replayGame() = 0;
	virtual void exitGame() = 0;
	virtual void loadMainMenu() = 0;
	virtual void loadOptionsMenu() = 0;
	virtual void notifyGameWin() = 0;
	virtual void notifyGameLose() = 0;
};