#pragma once

#include <ctime>

class MovingEntity;

enum class EntityStateType
{
	IDLE,
	FOLLOW_PATH,
	MOVE
};

class EntityState
{
public:
	virtual ~EntityState() {}

	virtual EntityStateType GetType() const = 0;
	virtual void Enter(MovingEntity& entity) = 0;
	virtual void Update(MovingEntity& entity, std::time_t elapsedTimeInMS) = 0;
	virtual void Exit(MovingEntity& entity) = 0;
};