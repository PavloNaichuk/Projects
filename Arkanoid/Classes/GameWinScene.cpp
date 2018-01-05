#include "GameWinScene.h"
#include "GameController.h"
#include "GameFactory.h"
#include "Utilities.h"

USING_NS_CC;

GameWinScene::GameWinScene(GameController* gameController)
	: _gameController(gameController)
{
}

GameWinScene* GameWinScene::create(GameController* gameController)
{
	auto scene = new (std::nothrow) GameWinScene(gameController);
	if ((scene != nullptr) && scene->init())
	{
		scene->autorelease();
	}
	else
	{
		delete scene;
		scene = nullptr;
	}
	return scene;
}

bool GameWinScene::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	auto statusLabelPos = Vec2(visibleSize.width / 2, visibleSize.height - 100);
	auto statusLabel = GameFactory::createLabel(statusLabelPos, 44, "Congratulations!");
	addChild(statusLabel, 1);

	auto userDefault = UserDefault::getInstance();
	auto playerScore = userDefault->getIntegerForKey(kPlayerScoreKey);

	char stringBuffer[80];
	std::snprintf(stringBuffer, sizeof(stringBuffer), "Your score: %d", playerScore);

	auto scoreLabelPos = Vec2(visibleSize.width / 2, visibleSize.height - 200);
	auto scoreLabel = GameFactory::createLabel(scoreLabelPos, 32, stringBuffer);
	addChild(scoreLabel, 1);

	auto replayItemPos = Vec2(visibleSize.width / 2, 50);
	auto replayItem = GameFactory::createMenuItem(replayItemPos, 32, "Replay");
	replayItem->setCallback([this](Ref* sender) {
		_gameController->replayGame();
	});
	replayItem->runAction(GameFactory::createMenuItemAnimation());

	auto replayMenu = Menu::create(replayItem, nullptr);
	replayMenu->setPosition(Vec2::ZERO);
	addChild(replayMenu, 1);

	return true;
}