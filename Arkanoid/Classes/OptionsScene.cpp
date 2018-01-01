#include "OptionsScene.h"

USING_NS_CC;

bool OptionsScene::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();

	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	TTFConfig optionsFontConfig;
	optionsFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	optionsFontConfig.fontSize = 40;

	auto optionsLabel = Label::createWithTTF(optionsFontConfig, "Options");
	optionsLabel->setTextColor(Color4B::WHITE);
	optionsLabel->enableShadow(Color4B::BLACK);
	optionsLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + visibleSize.height / 4));
	addChild(optionsLabel, 1);

	return true;
}
