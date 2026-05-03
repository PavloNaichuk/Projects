#include "Ghost.h"
#include "AI/GhostAI.h"
#include "Core/MessageCenter.h"

Ghost::Ghost(EntityType type, const Vector2f& center, const Vector2f& halfSize, Heading heading, float speed,
		std::shared_ptr<CollisionDetector> collisionDetector, std::shared_ptr<GhostAI> AI)
	: MovingEntity(type, center, halfSize, heading, speed, collisionDetector)
	, m_AI(AI)
{
}

void Ghost::Update(std::time_t elapsedTimeInMS)
{
	MovingEntity::Update(elapsedTimeInMS);
	m_AI->Update(*this);
}

void Ghost::Kill()
{
	MessageCenter::GetInstance().AddMessage({EntityKilled, this, nullptr});
}

void Ghost::Respawn(const Vector2f& center, Heading heading)
{
	Stop();
	SetCenter(center);
	SetHeading(heading);
	m_AI->Respawn(*this);
}
