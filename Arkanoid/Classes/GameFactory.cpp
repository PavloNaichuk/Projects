#include "GameFactory.h"
#include "Utilities.h"

Sprite* GameFactory::createBrick(int lives, const Vec2& position, const Size& size)
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

Sprite* GameFactory::createLeftWall(const Vec2& position, const Size& size)
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

Sprite* GameFactory::createRightWall(const Vec2& position, const Size& size)
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

Sprite* GameFactory::createTopWall(const Vec2& position, const Size& size)
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

Sprite* GameFactory::createPaddle(const Vec2& position, const Size& size)
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

Sprite* GameFactory::createBall(const Vec2& position, float radius)
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

Node* GameFactory::createExitZone(const Vec2& start, const Vec2& end)
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

Label* GameFactory::createLabel(const Vec2& position, float fontSize, const std::string& text)
{
	TTFConfig fontConfig;
	fontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	fontConfig.fontSize = fontSize;

	auto label = Label::createWithTTF(fontConfig, text);
	label->setTextColor(Color4B::WHITE);
	label->enableShadow(Color4B::BLACK);
	label->setPosition(position);

	return label;
}

MenuItem* GameFactory::createMenuItem(const Vec2& position, float fontSize, const std::string& text)
{
	TTFConfig fontConfig;
	fontConfig.fontFilePath = "fonts/Marker Felt.ttf";
	fontConfig.fontSize = fontSize;

	auto label = Label::createWithTTF(fontConfig, text);
	label->setTextColor(Color4B::WHITE);
	label->enableShadow(Color4B::BLACK);

	auto menuItem = MenuItemLabel::create(label);
	menuItem->setPosition(position);

	return menuItem;
}

Action* GameFactory::createMenuItemAnimation()
{
	auto scaleUp = ScaleBy::create(2.0f, 1.1f);
	auto scaleDown = scaleUp->reverse();
	auto delay = DelayTime::create(0.25f);
	auto sequence = Sequence::create(scaleUp, delay, scaleDown, delay->clone(), nullptr);

	return RepeatForever::create(sequence);
}

TransitionScene* GameFactory::createTransitionScene(Scene* scene)
{
	return TransitionFade::create(2.0f, scene);
}
