#pragma once

#include "cocos2d.h"

USING_NS_CC;

enum BrickType
{
	RED_BRICK = 0,
	GREEN_BRICK,
	BLUE_BRICK,
	NUM_BRICK_TYPES
};

const Size operator* (const Size& lhs, const Size& rhs);