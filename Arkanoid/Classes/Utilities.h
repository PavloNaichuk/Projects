#pragma once

#include "cocos2d.h"

USING_NS_CC;

enum EntityType
{
	RED_BRICK = 1 << 0,
	GREEN_BRICK = 1 << 1,
	BLUE_BRICK = 1 << 2,
	BORDER = 1 << 3,
	PADDLE = 1 << 4,
	BALL = 1 << 5
};

const Size operator* (const Size& lhs, const Size& rhs);