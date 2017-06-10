#pragma once

#include "MovingEntity.h"
#include <memory>

enum class BehaviorMode
{
	NORMAL,
	POWERED
};

class EntityManager;

class PacMan : public MovingEntity
{
public:
	PacMan(const Vector2f& center, const Vector2f& halfSize, Heading heading,
		float speed, unsigned lives, std::shared_ptr<CollisionDetector> collisionDetector,
		std::shared_ptr<EntityManager> entityManager);

	PacMan(const PacMan&) = delete;
	PacMan& operator= (const PacMan&) = delete;

	void Update(std::time_t elapsedTimeInMS) override;
	void Kill() override;
	void Respawn(const Vector2f& center, Heading heading) override;

	BehaviorMode GetBehaviorMode() const { return m_BehaviorMode; }
	void SetBehaviorMode(BehaviorMode behaviorMode);

	unsigned GetLives() const { return m_Lives; }

	unsigned GetScore() const { return m_Score; }
	void AddScore(unsigned score);

private:
	void KillGhostIfPossible();

private:
	unsigned m_Lives;
	unsigned m_Score;
	BehaviorMode m_BehaviorMode;
	std::shared_ptr<EntityManager> m_EntityManager;
};