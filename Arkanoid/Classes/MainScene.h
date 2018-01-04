#pragma once

#include "cocos2d.h"

USING_NS_CC;

class GameController;

class MainScene : public Scene
{
public:
	MainScene(GameController* gameController);
	static MainScene* create(GameController* gameController);
	
	bool init() override;

private:
	GameController* _gameController;
};

