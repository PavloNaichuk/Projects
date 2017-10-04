#include "PowerUpToLevel2.h"
#include "GameWorld.h"
#include "Collision.h"

PowerUpToLevel2::PowerUpToLevel2(bool isActive, const Point& center, const Size& size, int scores)
	: Trigger(isActive, center, size)
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