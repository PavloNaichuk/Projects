#pragma once

#include "cocos2d.h"

class GameController;

USING_NS_CC;

class OptionsScene : public Scene
{
public:
	OptionsScene(GameController* gameController);
	static OptionsScene* create(GameController* gameController);

	bool init() override;

private:
	GameController* _gameController;
};