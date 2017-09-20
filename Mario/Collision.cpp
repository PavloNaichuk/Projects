#include "Collision.h"
#include "GameUnit.h"

bool Collide(const GameUnit& gameUnit1, const GameUnit& gameUnit2)
{
	Point topLeft1 = gameUnit1.mCenter - gameUnit1.mHalfSize;
	Point bottomRight1 = gameUnit1.mCenter + gameUnit1.mHalfSize;

	Point topLeft2 = gameUnit2.mCenter - gameUnit2.mHalfSize;
	Point bottomRight2 = gameUnit2.mCenter + gameUnit2.mHalfSize;

	return (bottomRight1.mX > topLeft2.mX) && 
		(bottomRight2.mX > topLeft1.mX) &&
		(bottomRight2.mY > topLeft1.mY) &&
		(bottomRight1.mY > topLeft2.mY);
}
