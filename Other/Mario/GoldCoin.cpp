#include "GoldCoin.h"
#include "GameWorld.h"
#include "Collision.h"

GoldCoin::GoldCoin(bool isActive, const Point& center, const Size& size, int scores)
	: Trigger(isActive, center, size)
	, mScores(scores)
{
}

void GoldCoin::Update(float elapsedTime, GameWorld& gameWorld)
{
	if (!mIsActive)
		return;
	
	if (Collide(*this, gameWorld.mMario))
	{
		gameWorld.mMario.mScores += mScores;
		mIsActive = false;
	}
}