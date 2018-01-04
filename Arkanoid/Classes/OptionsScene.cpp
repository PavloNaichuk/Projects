#include "OptionsScene.h"
#include "GameController.h"
#include "Utilities.h"

USING_NS_CC;

OptionsScene::OptionsScene(GameController* gameController)
	: _gameController(gameController)
{
}

OptionsScene* OptionsScene::create(GameController* gameController)
{
	auto scene = new (std::nothrow) OptionsScene(gameController);
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

bool OptionsScene::init()
{
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	
	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	TTFConfig titleFontConfig;
	titleFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	titleFontConfig.fontSize = 44;

	auto titleLabel = Label::createWithTTF(titleFontConfig, "Options");
	titleLabel->setTextColor(Color4B::WHITE);
	titleLabel->enableShadow(Color4B::BLACK);
	titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
	addChild(titleLabel, 1);

	TTFConfig optionFontConfig;
	optionFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	optionFontConfig.fontSize = 32;

	const char* options[] =
	{
		"Move left - left arrow",
		"Move right - right arrow"
	};
	const int numOptions = sizeof(options) / sizeof(options[0]);
	
	const float spaceBetweenOptions = 10.0f;
	Vec2 optionPos(visibleSize.width / 2, visibleSize.height - 200);
	for (int optionIndex = 0; optionIndex < numOptions; ++optionIndex)
	{
		auto optionLabel = Label::createWithTTF(optionFontConfig, options[optionIndex]);
		optionLabel->setTextColor(Color4B::WHITE);
		optionLabel->enableShadow(Color4B::BLACK);
		optionLabel->setPosition(optionPos);
		
		addChild(optionLabel, 1);

		optionPos.y -= optionLabel->getContentSize().height;
		optionPos.y -= spaceBetweenOptions;
	}

	TTFConfig backFontConfig;
	backFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	backFontConfig.fontSize = 32;

	auto backLabel = Label::createWithTTF(backFontConfig, "Back");
	backLabel->setTextColor(Color4B::WHITE);
	backLabel->enableShadow(Color4B::BLACK);

	auto backItem = MenuItemLabel::create(backLabel);
	backItem->setPosition(Vec2(visibleSize.width / 2, 50));
	backItem->setCallback([this](Ref* sender) {
		_gameController->loadMainMenu();
	});
	backItem->runAction(createMenuItemAnimation());

	auto backMenu = Menu::create(backItem, nullptr);
	backMenu->setPosition(Vec2::ZERO);
	addChild(backMenu, 1);

	return true;
}
