#pragma once

#include <memory>
#include <ctime>

class EntityState;
class MovingEntity;
class CollisionDetector;

class EntityStateMachine
{
public:
	using StatePointer = std::shared_ptr<EntityState>;
	
	EntityStateMachine(std::shared_ptr<CollisionDetector> collisionDetector, MovingEntity& entity);
		
	StatePointer GetIdleState() { return m_IdleState; }
	StatePointer GetMoveState() { return m_MoveState; }
	StatePointer GetFollowPathState() { return m_FollowPathState; }
	
	StatePointer GetCurrentState() { return m_CurrState; }
	void ChangeState(StatePointer newState);
	
	void Update(std::time_t elapsedTimeInMS);

private:
	MovingEntity& m_Entity;

	StatePointer m_IdleState;
	StatePointer m_MoveState;
	StatePointer m_FollowPathState;
	StatePointer m_CurrState;
};