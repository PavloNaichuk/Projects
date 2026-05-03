#include "PowerUpToLevel3.h"
#include "GameWorld.h"
#include "Collision.h"

PowerUpToLevel3::PowerUpToLevel3(bool isActive, const Point& center, const Size& size, int scores)
	: Trigger(isActive, center, size)
	, mScores(scores)
{
}

void PowerUpToLevel3::Update(float elapsedTime, GameWorld& gameWorld)
{
	if (!mIsActive)
		return;

	if (Collide(*this, gameWorld.mMario))
	{
		gameWorld.mMario.mScores += mScores;
		gameWorld.mMario.SetSkillLevel(SkillLevels::LEVEL3);
		mIsActive = false;
	}
}