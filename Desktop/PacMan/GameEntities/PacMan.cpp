#include "PacMan.h"
#include "Core/MessageCenter.h"
#include "GameEntities/EntityManager.h"
#include <cassert>

PacMan::PacMan(const Vector2f& center, const Vector2f& halfSize, Heading heading,
		float speed, unsigned lives, std::shared_ptr<CollisionDetector> collisionDetector,
		std::shared_ptr<EntityManager> entityManager)
	: MovingEntity(EntityType::PACMAN, center, halfSize, heading, speed, collisionDetector)
	, m_Lives(lives)
	, m_Score(0)
	, m_BehaviorMode(BehaviorMode::NORMAL)
	, m_EntityManager(entityManager)
{}

void PacMan::SetBehaviorMode(BehaviorMode behaviorMode)
{
	if (m_BehaviorMode != behaviorMode)
	{
		m_BehaviorMode = behaviorMode;
		MessageCenter::GetInstance().AddMessage({BehaviorChanged, this, nullptr});
	}
}

void PacMan::Update(std::time_t elapsedTimeInMS)
{
	MovingEntity::Update(elapsedTimeInMS);
	KillGhostIfPossible();
}

void PacMan::Respawn(const Vector2f& center, Heading heading)
{
	Stop();
	SetCenter(center);
	SetHeading(heading);
}

void PacMan::Kill()
{
	if (m_Lives > 0)
	{
		--m_Lives;
		MessageCenter::GetInstance().AddMessage({EntityKilled, this, nullptr});
	}
}

void PacMan::AddScore(unsigned score)
{
	m_Score += score;
	MessageCenter::GetInstance().AddMessage({ScoreChanged, this, nullptr});
}

void PacMan::KillGhostIfPossible()
{
	if (m_BehaviorMode != BehaviorMode::POWERED)
		return;

	for (auto entity : m_EntityManager->GetMovingEntities())
	{
		if (entity->GetType() != EntityType::PACMAN)
		{
			if (Overlap(*this, *entity))
			{
				entity->Kill();
				return;
			}
		}
	}
}
