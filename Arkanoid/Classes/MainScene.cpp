#include "MainScene.h"
#include "GameController.h"
#include "GameFactory.h"
#include "Utilities.h"

USING_NS_CC;

MainScene::MainScene(GameController* gameController)
	: _gameController(gameController)
{
}

MainScene* MainScene::create(GameController* gameController)
{
	auto scene = new (std::nothrow) MainScene(gameController);
	if ((scene != nullptr) && (scene->init()))
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

bool MainScene::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	
	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);
	
	auto logoLabelPos = Vec2(visibleSize.width / 2, visibleSize.height / 2 + visibleSize.height / 4);
	auto logoLabel = GameFactory::createLabel(logoLabelPos, 60, "Arkanoid");
	addChild(logoLabel, 1);
	
	auto offsetBetweenMenuItems = Vec2(0.0f, 50.0f);
	auto startItemPos = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	auto startItem = GameFactory::createMenuItem(startItemPos, 32, "Start Game");
	startItem->setCallback([this](Ref* sender) {
		_gameController->playGame();
	});
	startItem->runAction(GameFactory::createMenuItemAnimation());
	
	auto optionsItemPos = startItemPos - offsetBetweenMenuItems;
	auto optionsItem = GameFactory::createMenuItem(optionsItemPos, 32, "Options");
	optionsItem->setCallback([this](Ref* sender) {
		_gameController->loadOptionsMenu();
	});
	optionsItem->runAction(GameFactory::createMenuItemAnimation());
	
	auto exitItemPos = optionsItemPos - offsetBetweenMenuItems;
	auto exitItem = GameFactory::createMenuItem(exitItemPos, 32, "Exit Game");
	exitItem->setCallback([this](Ref* sender) {
		_gameController->exitGame();
	});
	exitItem->runAction(GameFactory::createMenuItemAnimation());

	Vector<MenuItem*> menuItems = {startItem, optionsItem, exitItem};
	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	return true;
}
