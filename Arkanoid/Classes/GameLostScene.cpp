#include "GameLostScene.h"

USING_NS_CC;

bool GameLostScene::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();

	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	TTFConfig statusFontConfig;
	statusFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	statusFontConfig.fontSize = 40;

	auto statusLabel = Label::createWithTTF(statusFontConfig, "Lost");
	statusLabel->setTextColor(Color4B::WHITE);
	statusLabel->enableShadow(Color4B::BLACK);
	statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + visibleSize.height / 4));
	addChild(statusLabel, 1);
	
	return true;
}