#include "OptionsScene.h"
#include "GameController.h"
#include "GameFactory.h"
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

	auto titleLabelPos = Vec2(visibleSize.width / 2, visibleSize.height - 100);
	auto titleLabel = GameFactory::createLabel(titleLabelPos, 44, "Options");
	addChild(titleLabel, 1);

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
		auto optionLabel = GameFactory::createLabel(optionPos, 32, options[optionIndex]);
		addChild(optionLabel, 1);

		optionPos.y -= optionLabel->getContentSize().height;
		optionPos.y -= spaceBetweenOptions;
	}

	auto backItemPos = Vec2(visibleSize.width / 2, 50);
	auto backItem = GameFactory::createMenuItem(backItemPos, 32, "Back");
	backItem->setCallback([this](Ref* sender) {
		_gameController->loadMainMenu();
	});
	backItem->runAction(GameFactory::createMenuItemAnimation());

	auto backMenu = Menu::create(backItem, nullptr);
	backMenu->setPosition(Vec2::ZERO);
	addChild(backMenu, 1);

	return true;
}
