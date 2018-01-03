#include "PlayScene.h"
#include "GameWinScene.h"
#include "GameLoseScene.h"
#include "Utilities.h"
#include "ConfigManager.h"

namespace
{
	void setBrickTexture(Sprite* brick, int brickLives)
	{
		const char* frameName = nullptr;
		if (brickLives == 1)
			frameName = "./GreenBrick";
		else if (brickLives == 2)
			frameName = "./RedBrick";
		else if (brickLives == 3)
			frameName = "./BlueBrick";
		assert(frameName != nullptr);

		auto spriteFrameCache = SpriteFrameCache::getInstance();
		auto spriteFrame = spriteFrameCache->getSpriteFrameByName(frameName);
		
		brick->setTexture(spriteFrame->getTexture());
		brick->setTextureRect(spriteFrame->getRect());
	}
}

USING_NS_CC;

bool PlayScene::init()
{
	if (!Scene::initWithPhysics())
		return false;

	auto director = Director::getInstance();
	auto configManager = ConfigManager::getInstance();

	_paddleSpeed = configManager->getPaddleSpeed();
	_ballSpeed = configManager->getBallSpeed();
	
	auto colorLayer = LayerColor::create(Color4B::BLUE);
	addChild(colorLayer, 0);

	TTFConfig scoreFontConfig;
	scoreFontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	scoreFontConfig.fontSize = 24;

	_scoreLabel = Label::createWithTTF(scoreFontConfig, "");
	_scoreLabel->setTextColor(Color4B::WHITE);
	_scoreLabel->enableShadow(Color4B::BLACK);
	_scoreLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
	_scoreLabel->setPosition(configManager->getScoreLabelPos());
	addChild(_scoreLabel);
	updateScore(0);

	const int maxBrickLives = configManager->getMaxBrickLives();
	const int numBrickRows = configManager->getNumBrickRows();
	const int numBricksInRow = configManager->getNumBricksInRow();
	const Vec2& firstBrickPos = configManager->getFirstBrickPos();
	const Size& brickSize = configManager->getBrickSize();

	for (int row = 0; row < numBrickRows; ++row)
	{
		for (int col = 0; col < numBricksInRow; ++col)
		{
			auto brickOffset = brickSize * Size(col, -row);
			auto brickLives = random(1, maxBrickLives);
			auto brickPos = firstBrickPos + brickOffset;

			auto brick = createBrick(brickLives, brickPos, brickSize);
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
	contactListener->onContactPostSolve = CC_CALLBACK_1(PlayScene::onContactPostSolve, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(PlayScene::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto physicsWorld = getPhysicsWorld();
	physicsWorld->setGravity(Vec2::ZERO);
	physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

	auto startGame = [this](float delay)
	{
		_ball->getPhysicsBody()->setVelocity(Vec2(0.0, _ballSpeed));
		_allowUserInput = true;

		unschedule("startGame");
	};
	scheduleOnce(startGame, 1.0f, "startGame");

	return true;
}

Sprite* PlayScene::createBrick(int lives, const Vec2& position, const Size& size)
{
	auto sprite = Sprite::create();
	sprite->setPosition(position);
	sprite->setTag(BRICK);
	sprite->setUserObject(BrickData::create(lives));
	setBrickTexture(sprite, lives);

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

void PlayScene::updateScore(int score)
{
	_score = score;

	static char stringBuffer[60];
	std::snprintf(stringBuffer, sizeof(stringBuffer), "Score: %d", _score);
	_scoreLabel->setString(stringBuffer);
}

void PlayScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{	
	if (!_allowUserInput)
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
	if (!_allowUserInput)
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

	return ((node1 != nullptr) && (node2 != nullptr)) &&
	 ((node1->getTag() | node2->getTag()) != (BALL | EXIT_ZONE));
}

bool PlayScene::onContactPostSolve(PhysicsContact& contact)
{
	auto node1 = contact.getShapeA()->getBody()->getNode();
	auto node2 = contact.getShapeB()->getBody()->getNode();

	if ((node1 != nullptr) && (node2 != nullptr))
	{
		if ((node1->getTag() | node2->getTag()) == (PADDLE | BALL))
		{
			Vec2 ballDir = _ball->getPosition() - _paddle->getPosition();
			_ball->getPhysicsBody()->setVelocity(_ballSpeed * ballDir.getNormalized());
		}
		else if ((node1->getTag() | node2->getTag()) == (BALL | BRICK))
		{
			auto node = (node1->getTag() == BRICK) ? node1 : node2;

			auto brick = static_cast<Sprite*>(node);
			auto brickData = static_cast<BrickData*>(brick->getUserObject());

			brickData->setLives(brickData->getLives() - 1);
			if (brickData->getLives() > 0)
			{
				setBrickTexture(brick, brickData->getLives());
			}
			else
			{
				brick->removeFromParent();
				++_numDestroyedBricks;

				auto configManager = ConfigManager::getInstance();
				updateScore(_score + configManager->getScore());

				const int numBricks = configManager->getNumBrickRows() * configManager->getNumBricksInRow();
				if (_numDestroyedBricks == numBricks)
				{
					auto userDefault = UserDefault::getInstance();
					userDefault->setIntegerForKey(kPlayerScoreKey, _score);

					auto goToGameWinScene = [this](float delay)
					{
						auto director = Director::getInstance();

						_paddle->getPhysicsBody()->setVelocity(Vec2::ZERO);
						_allowUserInput = false;

						auto nextScene = GameWinScene::create();
						director->replaceScene(TransitionFade::create(2.0f, nextScene));

						unschedule("goToGameWinScene");
					};
					scheduleOnce(goToGameWinScene, 0.5f, "goToGameWinScene");
				}
				else
				{
					if ((_numDestroyedBricks % 5) == 0)
					{
						_ballSpeed += 0.1f * _ballSpeed;

						auto ballPhysicsBody = _ball->getPhysicsBody();
						Vec2 ballVelocity = ballPhysicsBody->getVelocity();

						ballPhysicsBody->setVelocity(_ballSpeed * ballVelocity.getNormalized());
					}
				}
			}
		}
	}
	return true;
}

bool PlayScene::onContactSeparate(PhysicsContact& contact)
{
	auto node1 = contact.getShapeA()->getBody()->getNode();
	auto node2 = contact.getShapeB()->getBody()->getNode();

	if ((node1 != nullptr) && (node2 != nullptr))
	{
		if ((node1->getTag() | node2->getTag()) == (BALL | EXIT_ZONE))
		{
			auto userDefault = UserDefault::getInstance();
			userDefault->setIntegerForKey(kPlayerScoreKey, _score);

			auto goToGameLoseScene = [this](float delay)
			{
				auto director = Director::getInstance();

				_paddle->getPhysicsBody()->setVelocity(Vec2::ZERO);
				_allowUserInput = false;

				auto nextScene = GameLoseScene::create();
				director->replaceScene(TransitionFade::create(2.0f, nextScene));

				unschedule("goToGameLoseScene");
			};
			scheduleOnce(goToGameLoseScene, 0.5f, "goToGameLoseScene");
		}
	}
	return true;
}
