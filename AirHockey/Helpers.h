#pragma once

#include "Vector.h"

struct Region
{
	Region();
	Region(const Point& topLeft, const Point& bottonRight);
	
	Point mTopLeft;
	Point mBottonRight;
};

struct BoardWall
{
	BoardWall(const Point& start, const Point& end, const Vector& orientation);

	Point mStart;
	Point mEnd;
	Vector mOrientation;
};
