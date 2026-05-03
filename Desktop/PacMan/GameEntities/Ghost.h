#pragma once

#include "MovingEntity.h"
#include <memory>

class GhostAI;

class Ghost : public MovingEntity
{
public:
	Ghost(EntityType type, const Vector2f& center, const Vector2f& halfSize, Heading heading, float speed,
		std::shared_ptr<CollisionDetector> collisionDetector, std::shared_ptr<GhostAI> AI);
	
	Ghost(const Ghost&) = delete;
	Ghost& operator= (const Ghost&) = delete;

	void Update(std::time_t elapsedTimeInMS) override;
	void Kill() override;
	void Respawn(const Vector2f& center, Heading heading) override;

private:
	std::shared_ptr<GhostAI> m_AI;
};