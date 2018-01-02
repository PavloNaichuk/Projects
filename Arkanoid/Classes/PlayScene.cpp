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

	auto bottomBorder = createBottomBorder(configManager->getBottomBorderStart(), configManager->getBottomBorderEnd());
	addChild(bottomBorder, 1);

	_paddle = createPaddle(configManager->getPaddlePos(), configManager->getPaddleSize());
	addChild(_paddle, 1);

	_ball = createBall(configManager->getBallPos(), configManager->getBallRadius());
	addChild(_ball, 1);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto physicsWorld = getPhysicsWorld();
	physicsWorld->setGravity(Vec2::ZERO);
	physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto setBallInitialVelocity = [this](float delay)
	{
		auto physicsBody = _ball->getPhysicsBody();
		physicsBody->setVelocity(Vec2(0.0, 200.0f));

		unschedule("setBallInitialVelocity");
	};
	scheduleOnce(setBallInitialVelocity, 1.0f, "setBallInitialVelocity");

	scheduleUpdate();
	return true;
}

void PlayScene::update(float deltaTime)
{
	//auto physicsBody = _paddle->getPhysicsBody();
	//physicsBody->setVelocity(deltaTime * _paddleVelocity);
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
	sprite->setTag(brickType);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(brickType);
	physicsBody->setCollisionBitmask(BALL);
	physicsBody->setContactTestBitmask(BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createLeftBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./LeftBorder");
	sprite->setPosition(position);
	sprite->setTag(BORDER);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createRightBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./RightBorder");
	sprite->setPosition(position);
	sprite->setTag(BORDER);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createTopBorder(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./TopBorder");
	sprite->setPosition(position);
	sprite->setTag(BORDER);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createPaddle(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Paddle");
	sprite->setPosition(position);
	sprite->setTag(PADDLE);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setMass(10.0f);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(PADDLE);
	physicsBody->setCollisionBitmask(BORDER | BALL);
	physicsBody->setContactTestBitmask(BALL);
	physicsBody->setRotationEnable(false);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createBall(const Vec2& position, float radius)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Ball");
	sprite->setPosition(position);
	sprite->setTag(BALL);

	auto physicsBody = PhysicsBody::createCircle(radius, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setMass(0.00001f);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(BALL);
	physicsBody->setCollisionBitmask(BORDER | PADDLE | RED_BRICK | GREEN_BRICK | BLUE_BRICK);
	physicsBody->setContactTestBitmask(PADDLE | RED_BRICK | GREEN_BRICK | BLUE_BRICK);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Node* PlayScene::createBottomBorder(const Vec2& start, const Vec2& end)
{
	auto node = Node::create();

	auto physicsBody = PhysicsBody::createEdgeSegment(start, end, PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setDynamic(false);
	physicsBody->setTag(BORDER);
	physicsBody->setCategoryBitmask(BORDER);
	physicsBody->setCollisionBitmask(BALL);

	node->setPhysicsBody(physicsBody);
	return node;
}

void PlayScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{	
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			auto physicsBody = _paddle->getPhysicsBody();
			physicsBody->setVelocity(Vec2(-250.0f, 0.0f));
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			auto physicsBody = _paddle->getPhysicsBody();
			physicsBody->setVelocity(Vec2(+250.0f, 0.0f));
			break;
		}
	}
}

void PlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			auto physicsBody = _paddle->getPhysicsBody();
			physicsBody->setVelocity(Vec2::ZERO);
			break;
		}
	}
}

bool PlayScene::onContactBegin(PhysicsContact& contact)
{
	auto node1 = contact.getShapeA()->getBody()->getNode();
	auto node2 = contact.getShapeB()->getBody()->getNode();

	if ((node1->getTag() | node2->getTag()) == (PADDLE | BALL))
	{
		auto ballPhysicsBody = _ball->getPhysicsBody();
		auto ballVelocity = ballPhysicsBody->getVelocity();
		float ballSpeed = ballVelocity.length();

		Vec2 dirBetweenCenters = _ball->getPosition() - _paddle->getPosition();
		dirBetweenCenters.normalize();

		ballPhysicsBody->setVelocity(ballSpeed * dirBetweenCenters);
	}

	return true;
}
