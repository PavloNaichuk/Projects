#pragma once

template <typename T1, typename T2>
bool Collide(const T1& gameObject1, const T2& gameObject2)
{
	Point topLeft1 = gameObject1.mCenter - gameObject1.mHalfSize;
	Point bottomRight1 = gameObject1.mCenter + gameObject1.mHalfSize;

	Point topLeft2 = gameObject2.mCenter - gameObject2.mHalfSize;
	Point bottomRight2 = gameObject2.mCenter + gameObject2.mHalfSize;

	return (bottomRight1.mX > topLeft2.mX) &&
		(bottomRight2.mX > topLeft1.mX) &&
		(bottomRight2.mY > topLeft1.mY) &&
		(bottomRight1.mY > topLeft2.mY);
}
