#include "GoldCoin.h"
#include "GameWorld.h"
#include "Collision.h"

GoldCoin::GoldCoin(State state, const Point& center, const Vector& velocity, const Size& size, int scores)
	: GameUnit(state, center, velocity, size)
	, mIsActive(true)
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