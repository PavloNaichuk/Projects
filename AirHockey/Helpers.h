#pragma once

#include "Vector.h"

struct Region
{
	Region();
	Region(const Point& topLeft, const Point& bottonRight);
	
	Point mTopLeft;
	Point mBottonRight;
};
