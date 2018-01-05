#pragma once

#include "cocos2d.h"

USING_NS_CC;

const char* const kPlayerScoreKey = "PlayerScore";

enum EntityType
{
	BRICK = 1 << 0,
	WALL = 1 << 1,
	PADDLE = 1 << 2,
	BALL = 1 << 3,
	EXIT_ZONE = 1 << 4
};

const Size operator* (const Size& lhs, const Size& rhs);

class BrickData : public Ref
{
public:
	BrickData(int lives);
	static BrickData* create(int lives);

	int getLives() const;
	void setLives(int lives);

private:
	int _lives;
};

void setBrickTexture(Sprite* brick, int brickLives);