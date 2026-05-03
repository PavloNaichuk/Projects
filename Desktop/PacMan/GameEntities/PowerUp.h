#pragma once

#include "Trigger.h"
#include <ctime>

class PowerUp : public Trigger
{
public:
	PowerUp(const Vector2f& center, const Vector2f& halfSize, bool active, unsigned scoreGiven);
	PowerUp(const PowerUp&) = delete;
	PowerUp& operator= (const PowerUp&) = delete;

	void Update(std::time_t elapsedTimeInMS) override;
	void Try(PacMan& pacMan) override;
	bool IsTouching(const PacMan& pacMan) const override;

private:
	unsigned m_ScoreGiven;
	std::time_t m_PowerUsageEndTimeInSec;
	PacMan* m_pPacMan;
};