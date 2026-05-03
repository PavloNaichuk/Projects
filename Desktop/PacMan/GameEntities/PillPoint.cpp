#include "PillPoint.h"
#include "PacMan.h"
#include "Core/MessageCenter.h"
#include <cmath>

PillPoint::PillPoint(const Vector2f& center, const Vector2f& halfSize, bool active, unsigned scoreGiven)
	: Trigger(EntityType::PILL_POINT, center, halfSize, active)
	, m_ScoreGiven(scoreGiven)
{}

void PillPoint::Update(std::time_t /*elapsedTimeInMS*/)
{
}

void PillPoint::Try(PacMan& pacMan)
{
	if (IsActive() && IsTouching(pacMan))
	{
		pacMan.AddScore(m_ScoreGiven);
		Deactivate();
		MessageCenter::GetInstance().AddMessage({PillPointChanged, this, nullptr});
	}
}

bool PillPoint::IsTouching(const PacMan& pacMan) const
{
	Vector2f pacManOffset = GetCenter() - pacMan.GetCenter();
	pacManOffset.m_X = std::abs(pacManOffset.m_X);
	pacManOffset.m_Y = std::abs(pacManOffset.m_Y);

	const Vector2f& halfSize = GetHalfSize();
	return (pacManOffset.m_X < halfSize.m_X) && (pacManOffset.m_Y < halfSize.m_Y);
}