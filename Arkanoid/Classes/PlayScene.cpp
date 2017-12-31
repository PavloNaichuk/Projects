#include "PlayScene.h"
#include "Utilities.h"
#include "ConfigManager.h"

USING_NS_CC;

Scene* PlayScene::create()
{
	auto director = Director::getInstance();
	auto configManager = ConfigManager::getInstance();

	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	auto scene = Scene::create();

	auto colorLayer = LayerColor::create(Color4B::BLUE);
	scene->addChild(colorLayer, 0);
	
	const int numBrickRows = configManager->getNumBrickRows();
	const int numBricksInRow = configManager->getNumBricksInRow();
	const Vec2& firstBrickPos = configManager->getFirstBrickPos();
	const Size& brickSize = configManager->getBrickSize();

	for (int row = 0; row < numBrickRows; ++row)
	{
		for (int col = 0; col < numBricksInRow; ++col)
		{
			auto brickOffset = brickSize * Size(col, -row);
			auto brickType = BrickType(random(0, NUM_BRICK_TYPES - 1));
			auto brickPos = firstBrickPos + brickOffset;

			auto brickSprite = createBrick(brickType, brickPos, brickSize);
			scene->addChild(brickSprite, 1);
		}
	}

	const Vec2& topBorderPos = configManager->getTopBorderPos();
	const Size& topBorderSize = configManager->getTopBorderSize();
	auto topBorder = createTopBorder(topBorderPos, topBorderSize);
	scene->addChild(topBorder, 1);

	const Vec2& leftBorderPos = configManager->getLeftBorderPos();
	const Size& leftBorderSize = configManager->getLeftBorderSize();
	auto leftBorder = createLeftBorder(leftBorderPos, leftBorderSize);
	scene->addChild(leftBorder, 1);

	const Vec2& rightBorderPos = configManager->getRightBorderPos();
	const Size& rightBorderSize = configManager->getRightBorderSize();
	auto rightBorder = createRightBorder(rightBorderPos, rightBorderSize);
	scene->addChild(rightBorder, 1);

	const Vec2& paddlePos = configManager->getPaddlePos();
	const Size& paddleSize = configManager->getPaddleSize();
	auto paddle = createPaddle(paddlePos, paddleSize);
	scene->addChild(paddle, 1);

	const Vec2& ballPos = configManager->getBallPos();
	const Size& ballSize = configManager->getBallSize();
	auto ball = createBall(ballPos, ballSize);
	scene->addChild(ball, 1);

	return scene;
}

Sprite* PlayScene::createBrick(BrickType brickType, const Vec2& position, const Size& size)
{
	const char* spriteFrameName = nullptr;
	if (brickType == RED_BRICK)
		spriteFrameName = "./RedBrick";
	else if (brickType == GREEN_BRICK)
		spriteFrameName = "./GreenBrick";
	else if (brickType == BLUE_BRICK)
		spriteFrameName = "./BlueBrick";
	assert(spriteFrameName != nullptr);

	auto sprite = Sprite::createWithSpriteFrameName(spriteFrameName);
	sprite->setPosition(position);

	return sprite;
}

Sprite* PlayScene::createLeftBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./LeftBorder");
	sprite->setPosition(position);

	return sprite;
}

Sprite* PlayScene::createRightBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./RightBorder");
	sprite->setPosition(position);

	return sprite;
}

Sprite* PlayScene::createTopBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./TopBorder");
	sprite->setPosition(position);

	return sprite;
}

Sprite* PlayScene::createPaddle(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Paddle");
	sprite->setPosition(position);

	return sprite;
}

Sprite* PlayScene::createBall(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Ball");
	sprite->setPosition(position);

	return sprite;
}
