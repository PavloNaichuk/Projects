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