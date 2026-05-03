#pragma once

#include "Common.h"
#include "GameUnit.h"

struct GameWorld;

enum class SkillLevels
{
	LEVEL1,
	LEVEL2,
	LEVEL3
};

class Mario : public GameUnit
{
public:
	Mario(State state, const Point& center, const Vector& velocity, const Size& size);

	void ProcessKeyPressed(SDL_Keycode key, GameWorld& gameWorld);
	void ProcessKeyReleased(SDL_Keycode key);
	void Update(float elapsedTime);
	void RemoveLife();

	SkillLevels GetSkillLevel() const;
	void SetSkillLevel(SkillLevels skillLevel);

	float mDirection;
	int mScores;
	int mLives;

private:
	SkillLevels mSkillLevel;
};
