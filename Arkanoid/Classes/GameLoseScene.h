#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameController;

class GameLoseScene : public Scene
{
public:
	GameLoseScene(GameController* gameController);
	static GameLoseScene* create(GameController* gameController);

	bool init() override;

private:
	GameController* _gameController;
};

