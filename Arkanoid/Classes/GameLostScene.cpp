#include "GameLostScene.h"

USING_NS_CC;

bool GameLostScene::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	TTFConfig statusFontConfig;
	statusFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	statusFontConfig.fontSize = 44;

	auto statusLabel = Label::createWithTTF(statusFontConfig, "Whooppss. You lost");
	statusLabel->setTextColor(Color4B::WHITE);
	statusLabel->enableShadow(Color4B::BLACK);
	statusLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
	addChild(statusLabel, 1);

	TTFConfig scoreFontConfig;
	scoreFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	scoreFontConfig.fontSize = 32;

	char stringBuffer[80];
	std::snprintf(stringBuffer, sizeof(stringBuffer), "Your score: %d", 10);
	auto scoreLabel = Label::createWithTTF(scoreFontConfig, stringBuffer);
	scoreLabel->setTextColor(Color4B::WHITE);
	scoreLabel->enableShadow(Color4B::BLACK);
	scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 200));
	addChild(scoreLabel, 1);

	TTFConfig replayFontConfig;
	replayFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	replayFontConfig.fontSize = 32;

	auto replayLabel = Label::createWithTTF(replayFontConfig, "Replay");
	replayLabel->setTextColor(Color4B::WHITE);
	replayLabel->enableShadow(Color4B::BLACK);

	auto replayItem = MenuItemLabel::create(replayLabel);
	replayItem->setPosition(Vec2(visibleSize.width / 2, 50));
	replayItem->setCallback([director](Ref* sender) {
	});

	auto replayMenu = Menu::create(replayItem, nullptr);
	replayMenu->setPosition(Vec2::ZERO);
	addChild(replayMenu, 1);
	
	return true;
}