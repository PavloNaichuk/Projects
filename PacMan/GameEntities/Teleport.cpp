#include "Teleport.h"
#include "PacMan.h"
#include <cmath>

Teleport::Teleport(const Vector2f& center, const Vector2f& halfSize, const Vector2f& exitCenter)
	: Trigger(EntityType::TELEPORT, center, halfSize, true)
	, m_ExitCenter(exitCenter)
{
}

void Teleport::Update(std::time_t /*elapsedTimeInMS*/)
{
}

void Teleport::Try(PacMan& pacMan)
{
}

bool Teleport::IsTouching(const PacMan& pacMan) const
{
	static const float allowedOffset = 12.0f;

	Vector2f pacManOffset = GetCenter() - pacMan.GetCenter();
	pacManOffset.m_X = std::abs(pacManOffset.m_X);
	pacManOffset.m_Y = std::abs(pacManOffset.m_Y);

	const Vector2f& halfSize = GetHalfSize();
	return (pacManOffset.m_X < allowedOffset) && (pacManOffset.m_Y < allowedOffset);
}
