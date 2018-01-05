#include "Utilities.h"

const Size operator* (const Size& lhs, const Size& rhs)
{
	return Size(lhs.width * rhs.width, lhs.height * rhs.height);
}

BrickData::BrickData(int lives)
	: _lives(lives)
{
}

BrickData* BrickData::create(int lives)
{
	auto brickData = new (std::nothrow) BrickData(lives);
	if (brickData != nullptr)
	{
		brickData->autorelease();
	}
	else
	{
		delete brickData;
		brickData = nullptr;
	}
	return brickData;
}

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

int BrickData::getLives() const
{
	return _lives;
}

void BrickData::setLives(int lives)
{
	_lives = lives;
}