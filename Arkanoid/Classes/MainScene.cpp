#include "MainScene.h"
#include "PlayScene.h"
#include "OptionsScene.h"

USING_NS_CC;

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
	startItem->setCallback([director](Ref* sender) {
		auto nextScene = PlayScene::create();
		director->replaceScene(TransitionFade::create(2.0f, nextScene));
	});

	auto optionsLabel = Label::createWithTTF(menuFontConfig, "Options");
	optionsLabel->setTextColor(Color4B::WHITE);
	optionsLabel->enableShadow(Color4B::BLACK);

	auto optionsItem = MenuItemLabel::create(optionsLabel);
	optionsItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
	optionsItem->setCallback([director](Ref* sender) {
		auto nextScene = OptionsScene::create();
		director->pushScene(TransitionFade::create(2.0f, nextScene));
	});

	auto exitLabel = Label::createWithTTF(menuFontConfig, "Exit Game");
	exitLabel->setTextColor(Color4B::WHITE);
	exitLabel->enableShadow(Color4B::BLACK);

	auto exitItem = MenuItemLabel::create(exitLabel);
	exitItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
	exitItem->setCallback([director](Ref* sender) {
		director->end();
	});

	Vector<MenuItem*> menuItems = {startItem, optionsItem, exitItem};
	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	return true;
}
