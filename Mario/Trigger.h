#pragma once

#include "Vector.h"

struct Trigger
{
public:
	Trigger(bool isActive, const Point& center, const Size& size);

	bool mIsActive;
	Point mCenter;
	Size mHalfSize;
};