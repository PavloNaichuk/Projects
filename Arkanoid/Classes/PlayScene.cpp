#include "PlayScene.h"
#include "GameWinScene.h"
#include "GameLoseScene.h"
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

	const int numBrickStates = 3;
	for (int row = 0; row < numBrickRows; ++row)
	{
		for (int col = 0; col < numBricksInRow; ++col)
		{
			auto brickOffset = brickSize * Size(col, -row);
			auto brickState = random(1, numBrickStates);
			auto brickPos = firstBrickPos + brickOffset;

			auto brick = createBrick(brickState, brickPos, brickSize);
			addChild(brick, 1);
		}
	}

	auto topWall = createTopWall(configManager->getTopWallPos(), configManager->getTopWallSize());
	addChild(topWall, 1);

	auto leftWall = createLeftWall(configManager->getLeftWallPos(), configManager->getLeftWallSize());
	addChild(leftWall, 1);

	auto rightWall = createRightWall(configManager->getRightWallPos(), configManager->getRightWallSize());
	addChild(rightWall, 1);

	auto exitZone = createExitZone(configManager->getExitZoneStart(), configManager->getExitZoneEnd());
	addChild(exitZone, 1);

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
		_ball->getPhysicsBody()->setVelocity(Vec2(0.0, _ballSpeed));
		_userInputAllowed = true;
		unschedule("setBallInitialVelocity");
	};
	scheduleOnce(setBallInitialVelocity, 0.5f, "setBallInitialVelocity");
	return true;
}

Sprite* PlayScene::createBrick(int brickState, const Vec2& position, const Size& size)
{
	const char* spriteFrameName = nullptr;
	if (brickState == 1)
		spriteFrameName = "./GreenBrick";
	else if (brickState == 2)
		spriteFrameName = "./RedBrick";
	else if (brickState == 3)
		spriteFrameName = "./BlueBrick";
	assert(spriteFrameName != nullptr);

	auto sprite = Sprite::createWithSpriteFrameName(spriteFrameName);
	sprite->setPosition(position);
	sprite->setTag(BRICK);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(BRICK);
	physicsBody->setCollisionBitmask(BALL);
	physicsBody->setContactTestBitmask(BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createLeftWall(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./LeftWall");
	sprite->setPosition(position);
	sprite->setTag(WALL);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createRightWall(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./RightWall");
	sprite->setPosition(position);
	sprite->setTag(WALL);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(PADDLE | BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createTopWall(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./TopWall");
	sprite->setPosition(position);
	sprite->setTag(WALL);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(WALL);
	physicsBody->setCollisionBitmask(BALL);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Sprite* PlayScene::createPaddle(const Vec2& position, const Size& size)
{
	auto sprite = Sprite::createWithSpriteFrameName("./Paddle");
	sprite->setPosition(position);
	sprite->setTag(PADDLE);

	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(1.0f, 0.0f, 0.0f));
	physicsBody->setMass(10.0f);
	physicsBody->setDynamic(true);
	physicsBody->setCategoryBitmask(PADDLE);
	physicsBody->setCollisionBitmask(WALL | BALL);
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
	physicsBody->setCollisionBitmask(WALL | PADDLE | BRICK | EXIT_ZONE);
	physicsBody->setContactTestBitmask(PADDLE | BRICK | EXIT_ZONE);

	sprite->setPhysicsBody(physicsBody);
	return sprite;
}

Node* PlayScene::createExitZone(const Vec2& start, const Vec2& end)
{
	auto node = Node::create();
	node->setTag(EXIT_ZONE);

	auto physicsBody = PhysicsBody::createEdgeSegment(start, end, PhysicsMaterial(1.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(EXIT_ZONE);
	physicsBody->setCollisionBitmask(BALL);
	physicsBody->setContactTestBitmask(BALL);

	node->setPhysicsBody(physicsBody);
	return node;
}

void PlayScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{	
	if (!_userInputAllowed)
		return;

	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			_paddle->getPhysicsBody()->setVelocity(Vec2(-_paddleSpeed, 0.0f));
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			_paddle->getPhysicsBody()->setVelocity(Vec2(_paddleSpeed, 0.0f));
			break;
		}
	}
}

void PlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (!_userInputAllowed)
		return;

	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			_paddle->getPhysicsBody()->setVelocity(Vec2::ZERO);
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
		Vec2 ballDir = _ball->getPosition() - _paddle->getPosition();
		_ball->getPhysicsBody()->setVelocity(_ballSpeed * ballDir.getNormalized());
		
		return true;
	}
	if ((node1->getTag() | node2->getTag()) == (BALL | EXIT_ZONE))
	{
		auto director = Director::getInstance();
		director->replaceScene(GameLoseScene::create());

		return false;
	}

	return true;
}
