#pragma once

#include "EntityState.h"
#include <memory>

class CollisionDetector;

class MoveState : public EntityState
{
public:
	MoveState(std::shared_ptr<CollisionDetector> collisionDetector);

	EntityStateType GetType() const override;
	void Enter(MovingEntity& entity) override;
	void Update(MovingEntity& entity, std::time_t elapsedTimeInMS) override;
	void Exit(MovingEntity& entity) override;

private:
	std::shared_ptr<CollisionDetector> m_CollisionDetector;
};