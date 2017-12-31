#include "PlayScene.h"
#include "Utilities.h"
#include "ConfigManager.h"

USING_NS_CC;

bool PlayScene::init()
{
	if (!Scene::initWithPhysics())
		return false;

	auto director = Director::getInstance();
	auto configManager = ConfigManager::getInstance();

	auto visibleSize = director->getVisibleSize();
	auto origin = director->getVisibleOrigin();

	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);
	
	const int numBrickRows = configManager->getNumBrickRows();
	const int numBricksInRow = configManager->getNumBricksInRow();
	const Vec2& firstBrickPos = configManager->getFirstBrickPos();
	const Size& brickSize = configManager->getBrickSize();

	const EntityType brickTypes[] = {RED_BRICK, GREEN_BRICK, BLUE_BRICK};
	const int numBrickTypes = sizeof(brickTypes) / sizeof(brickTypes[0]);

	for (int row = 0; row < numBrickRows; ++row)
	{
		for (int col = 0; col < numBricksInRow; ++col)
		{
			auto brickOffset = brickSize * Size(col, -row);
			auto brickType = brickTypes[random(0, numBrickTypes - 1)];
			auto brickPos = firstBrickPos + brickOffset;

			auto brickSprite = createBrick(brickType, brickPos, brickSize);
			addChild(brickSprite, 1);
		}
	}

	auto topBorder = createTopBorder(configManager->getTopBorderPos(), configManager->getTopBorderSize());
	addChild(topBorder, 1);

	auto leftBorder = createLeftBorder(configManager->getLeftBorderPos(), configManager->getLeftBorderSize());
	addChild(leftBorder, 1);

	auto rightBorder = createRightBorder(configManager->getRightBorderPos(), configManager->getRightBorderSize());
	addChild(rightBorder, 1);

	auto paddle = createPaddle(configManager->getPaddlePos(), configManager->getPaddleSize());
	addChild(paddle, 1);

	auto ball = createBall(configManager->getBallPos(), configManager->getBallRadius());
	addChild(ball, 1);

	return true;
}

Sprite* PlayScene::createBrick(EntityType brickType, const Vec2& position, const Size& size)
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

	auto physicsBody = PhysicsBody::createBox(size, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(false);
	physicsBody->setTag(brickType);
	physicsBody->setCategoryBitmask(brickType);
	physicsBody->setCollisionBitmask(BALL);

	sprite->addComponent(physicsBody);
	return sprite;
}

Sprite* PlayScene::createLeftBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./LeftBorder");
	sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createBox(size, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(false);
	physicsBody->setTag(BORDER);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->addComponent(physicsBody);
	return sprite;
}

Sprite* PlayScene::createRightBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./RightBorder");
	sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createBox(size, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(false);
	physicsBody->setTag(BORDER);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->addComponent(physicsBody);
	return sprite;
}

Sprite* PlayScene::createTopBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./TopBorder");
	sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createBox(size, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(false);
	physicsBody->setTag(BORDER);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->addComponent(physicsBody);
	return sprite;
}

Sprite* PlayScene::createPaddle(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Paddle");
	sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createBox(size, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(true);
	physicsBody->setTag(PADDLE);
	physicsBody->setCategoryBitmask(PADDLE);
	physicsBody->setCollisionBitmask(BORDER | BALL);

	sprite->addComponent(physicsBody);
	return sprite;
}

Sprite* PlayScene::createBall(const Vec2& position, float radius)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Ball");
	sprite->setPosition(position);

	auto physicsBody = PhysicsBody::createCircle(radius, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(true);
	physicsBody->setTag(BALL);
	physicsBody->setCategoryBitmask(BALL);
	physicsBody->setCollisionBitmask(BORDER | PADDLE);

	sprite->addComponent(physicsBody);
	return sprite;
}
