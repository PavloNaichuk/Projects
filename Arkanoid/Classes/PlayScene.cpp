#include "PlayScene.h"
#include "ConfigManager.h"
#include "GameController.h"
#include "GameFactory.h"
#include "Utilities.h"

USING_NS_CC;

PlayScene::PlayScene(GameController* gameController)
	: _gameController(gameController)
{
}

PlayScene* PlayScene::create(GameController* gameController)
{
	auto scene = new (std::nothrow) PlayScene(gameController);
	if ((scene != nullptr) && scene->init())
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

			auto brick = GameFactory::createBrick(brickLives, brickPos, brickSize);
			addChild(brick, 1);
		}
	}

	auto topWall = GameFactory::createTopWall(configManager->getTopWallPos(), configManager->getTopWallSize());
	addChild(topWall, 1);

	auto leftWall = GameFactory::createLeftWall(configManager->getLeftWallPos(), configManager->getLeftWallSize());
	addChild(leftWall, 1);

	auto rightWall = GameFactory::createRightWall(configManager->getRightWallPos(), configManager->getRightWallSize());
	addChild(rightWall, 1);

	auto exitZone = GameFactory::createExitZone(configManager->getExitZoneStart(), configManager->getExitZoneEnd());
	addChild(exitZone, 1);

	_paddle = GameFactory::createPaddle(configManager->getPaddlePos(), configManager->getPaddleSize());
	addChild(_paddle, 1);

	_ball = GameFactory::createBall(configManager->getBallPos(), configManager->getBallRadius());
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

	auto allowStart = [this](float delay)
	{
		_ball->getPhysicsBody()->setVelocity(Vec2(0.0, _ballSpeed));
		_allowUserInput = true;

		unschedule("allowStart");
	};
	scheduleOnce(allowStart, 1.0f, "allowStart");

	return true;
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

					auto notifyGameWin = [this](float delay)
					{
						_paddle->getPhysicsBody()->setVelocity(Vec2::ZERO);
						_allowUserInput = false;

						_gameController->notifyGameWin();
						unschedule("notifyGameWin");
					};
					scheduleOnce(notifyGameWin, 0.5f, "notifyGameWin");
				}
				else
				{
					if ((_numDestroyedBricks % 5) == 0)
					{
						_ballSpeed += configManager->getBallSpeedPercentChange() * _ballSpeed;

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

			auto notifyGameLose = [this](float delay)
			{
				_paddle->getPhysicsBody()->setVelocity(Vec2::ZERO);
				_allowUserInput = false;

				_gameController->notifyGameLose();
				unschedule("notifyGameLose");
			};
			scheduleOnce(notifyGameLose, 0.5f, "notifyGameLose");
		}
	}
	return true;
}
