#include "Trigger.h"

Trigger::Trigger(bool isActive, const Point& center, const Size& size)
	: mIsActive(isActive)
	, mCenter(center)
	, mHalfSize(0.5f * size)
{
}