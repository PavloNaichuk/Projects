#include "pch.h"
#include "Helpers.h"

Region::Region() 
{
}

Region::Region(const Point& topLeft, const Point& bottonRight) 
	: mTopLeft(topLeft)
	, mBottonRight(bottonRight)
{
}

BoardWall::BoardWall(const Point& start, const Point& end, const Vector& orientation) 
	: mStart(start)
	, mEnd(end)
	, mOrientation(orientation)
{
}