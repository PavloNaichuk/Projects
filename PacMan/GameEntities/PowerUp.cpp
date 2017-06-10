#include "PowerUp.h"
#include "PacMan.h"
#include "Core/MessageCenter.h"
#include "Core/Config.h"
#include <cmath>

PowerUp::PowerUp(const Vector2f& center, const Vector2f& halfSize, bool active, unsigned scoreGiven)
	: Trigger(EntityType::POWER_UP, center, halfSize, active)
	, m_ScoreGiven(scoreGiven)
	, m_PowerUsageEndTimeInSec(0)
	, m_pPacMan(nullptr)
{
}

void PowerUp::Update(std::time_t /*elapsedTimeInMS*/)
{
	if (m_pPacMan != nullptr)
	{
		std::time_t currTimeInSec = std::time(nullptr);
		if (currTimeInSec > m_PowerUsageEndTimeInSec)
		{
			m_pPacMan->SetBehaviorMode(BehaviorMode::NORMAL);
			m_pPacMan = nullptr;
		}
	}
}

void PowerUp::Try(PacMan& pacMan)
{
	if (IsActive() && IsTouching(pacMan))
	{
		m_pPacMan = &pacMan;
		m_pPacMan->AddScore(m_ScoreGiven);
		m_pPacMan->SetBehaviorMode(BehaviorMode::POWERED);

		Deactivate();
		MessageCenter::GetInstance().AddMessage({PowerUpChanged, this, nullptr});

		std::time_t currTimeInSec = std::time(nullptr);
		m_PowerUsageEndTimeInSec = currTimeInSec + Config::POWER_UP_DURATION_IN_SEC;
	}
}

bool PowerUp::IsTouching(const PacMan& pacMan) const
{
	Vector2f pacManOffset = GetCenter() - pacMan.GetCenter();
	pacManOffset.m_X = std::abs(pacManOffset.m_X);
	pacManOffset.m_Y = std::abs(pacManOffset.m_Y);

	const Vector2f& halfSize = GetHalfSize();
	return (pacManOffset.m_X < halfSize.m_X) && (pacManOffset.m_Y < halfSize.m_Y);
}