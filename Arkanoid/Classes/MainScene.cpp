#include "MainScene.h"
#include "GameController.h"
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
	auto origin = director->getVisibleOrigin();
	
	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	TTFConfig logoFontConfig;
	logoFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	logoFontConfig.fontSize = 60;

	auto logoLabel = Label::createWithTTF(logoFontConfig, "Arkanoid");
	logoLabel->setTextColor(Color4B::WHITE);
	logoLabel->enableShadow(Color4B::BLACK);
	logoLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + visibleSize.height / 4));
	addChild(logoLabel, 1);

	TTFConfig menuFontConfig;
	menuFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	menuFontConfig.fontSize = 32;

	auto startLabel = Label::createWithTTF(menuFontConfig, "Start Game");
	startLabel->setTextColor(Color4B::WHITE);
	startLabel->enableShadow(Color4B::BLACK);

	auto startItem = MenuItemLabel::create(startLabel);
	startItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	startItem->setCallback([this](Ref* sender) {
		_gameController->playGame();
	});
	startItem->runAction(createMenuItemAnimation());

	auto optionsLabel = Label::createWithTTF(menuFontConfig, "Options");
	optionsLabel->setTextColor(Color4B::WHITE);
	optionsLabel->enableShadow(Color4B::BLACK);

	auto optionsItem = MenuItemLabel::create(optionsLabel);
	optionsItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
	optionsItem->setCallback([this](Ref* sender) {
		_gameController->loadOptionsMenu();
	});
	optionsItem->runAction(createMenuItemAnimation());

	auto exitLabel = Label::createWithTTF(menuFontConfig, "Exit Game");
	exitLabel->setTextColor(Color4B::WHITE);
	exitLabel->enableShadow(Color4B::BLACK);

	auto exitItem = MenuItemLabel::create(exitLabel);
	exitItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
	exitItem->setCallback([this](Ref* sender) {
		_gameController->exitGame();
	});
	exitItem->runAction(createMenuItemAnimation());

	Vector<MenuItem*> menuItems = {startItem, optionsItem, exitItem};
	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	return true;
}
