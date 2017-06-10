#pragma once

#include "Trigger.h"

class Teleport : public Trigger
{
public:
	Teleport(const Vector2f& center, const Vector2f& halfSize, const Vector2f& exitCenter);
	Teleport(const Teleport&) = delete;
	Teleport& operator= (const Teleport&) = delete;

	void Update(std::time_t elapsedTimeInMS) override;
	void Try(PacMan& pacMan) override;
	bool IsTouching(const PacMan& pacMan) const override;

private:
	const Vector2f m_ExitCenter;
};