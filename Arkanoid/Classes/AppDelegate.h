#pragma once

#include "cocos2d.h"
#include "GameController.h"

USING_NS_CC;

class  AppDelegate : public Application, public GameController
{
public:
    AppDelegate();
    ~AppDelegate() override;

	// Application
    void initGLContextAttrs() override;
	bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;

	// GameController
	void startGame() override;
	void playGame() override;
	void replayGame() override;
	void exitGame() override;
	void loadMainMenu() override;
	void loadOptionsMenu() override;
	void notifyGameWin() override;
	void notifyGameLose() override;
};
