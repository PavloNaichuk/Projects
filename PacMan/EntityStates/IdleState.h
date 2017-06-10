#pragma once

#include "EntityState.h"

class IdleState : public EntityState
{
public:
	EntityStateType GetType() const override
	{
		return EntityStateType::IDLE;
	}
	void Enter(MovingEntity& /*entity*/) override
	{
	}
	void Update(MovingEntity& /*entity*/, std::time_t /*elapsedTimeInMS*/) override
	{
	}
	void Exit(MovingEntity& /*entity*/) override
	{
	}
};