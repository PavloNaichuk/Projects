#include "Utilities.h"

const Size operator* (const Size& lhs, const Size& rhs)
{
	return Size(lhs.width * rhs.width, lhs.height * rhs.height);
}

BrickData* BrickData::create(int lives)
{
	auto brickData = new (std::nothrow) BrickData();
	if (brickData != nullptr)
	{
		brickData->setLives(lives);
		brickData->autorelease();
	}
	else
	{
		delete brickData;
	}
	return brickData;
}

int BrickData::getLives() const
{
	return _lives;
}

void BrickData::setLives(int lives)
{
	_lives = lives;
}