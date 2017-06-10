#pragma once

#include "Trigger.h"

class PillPoint : public Trigger
{
public:
	PillPoint(const Vector2f& center, const Vector2f& halfSize, bool active, unsigned scoreGiven);
	PillPoint(const PillPoint&) = delete;
	PillPoint& operator= (const PillPoint&) = delete;

	void Update(std::time_t elapsedTimeInMS) override;
	void Try(PacMan& pacMan) override;
	bool IsTouching(const PacMan& pacMan) const override;
	
private:
	unsigned m_ScoreGiven;
};