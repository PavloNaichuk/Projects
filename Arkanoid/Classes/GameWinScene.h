#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameController;

class GameWinScene : public Scene
{
public:
	GameWinScene(GameController* gameController);
	static GameWinScene* create(GameController* gameController);

	bool init() override;

private:
	GameController* _gameController;
};
