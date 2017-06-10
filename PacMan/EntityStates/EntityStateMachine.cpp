#include "EntityStateMachine.h"
#include "IdleState.h"
#include "FollowPathState.h"
#include "MoveState.h"
#include <cassert>

EntityStateMachine::EntityStateMachine(std::shared_ptr<CollisionDetector> collisionDetector, MovingEntity& entity)
	: m_Entity(entity)
	, m_IdleState(new IdleState())
	, m_MoveState(new MoveState(collisionDetector))
	, m_FollowPathState(new FollowPathState(collisionDetector))
	, m_CurrState(m_IdleState)
{
}

void EntityStateMachine::Update(std::time_t elapsedTimeInMS)
{
	m_CurrState->Update(m_Entity, elapsedTimeInMS);
}

void EntityStateMachine::ChangeState(StatePointer newState)
{
	if (m_CurrState != newState)
	{
		m_CurrState->Exit(m_Entity);
		m_CurrState = newState;
		m_CurrState->Enter(m_Entity);
	}
}
