#include "PowerUpToLevel2.h"
#include "GameWorld.h"
#include "Collision.h"

PowerUpToLevel2::PowerUpToLevel2(State state, const Point& center, const Vector& velocity, const Size& size, int scores)
	: GameUnit(state, center, velocity, size)
	, mIsActive(true)
	, mScores(scores)
{
}

void PowerUpToLevel2::Update(float elapsedTime, GameWorld& gameWorld)
{
	if (!mIsActive)
		return;

	if (Collide(*this, gameWorld.mMario))
	{
		gameWorld.mMario.mScores += mScores;
		gameWorld.mMario.SetSkillLevel(SkillLevels::LEVEL2);
		mIsActive = false;
	}
}