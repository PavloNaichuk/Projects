#include "MovingEntity.h"
#include "EntityStates/EntityStateMachine.h"
#include <cmath>

MovingEntity::MovingEntity(EntityType type, const Vector2f& center, const Vector2f& halfSize,
	Heading heading, float speed, std::shared_ptr<CollisionDetector> collisionDetector)
	: BaseEntity(type, center, halfSize)
	, m_Heading(heading)
	, m_Speed(speed)
	, m_StateMachine(new EntityStateMachine(collisionDetector, *this))
{}

void MovingEntity::Update(std::time_t elapsedTimeInMS)
{
	m_StateMachine->Update(elapsedTimeInMS);
}

void MovingEntity::Move()
{
	m_StateMachine->ChangeState(m_StateMachine->GetMoveState());
}

void MovingEntity::Stop()
{
	m_StateMachine->ChangeState(m_StateMachine->GetIdleState());
}

void MovingEntity::FollowPath(std::shared_ptr<Path> path)
{
	m_Path = std::move(path);
	m_StateMachine->ChangeState(m_StateMachine->GetFollowPathState());
}

EntityStateType MovingEntity::GetCurrentState()
{
	return m_StateMachine->GetCurrentState()->GetType();
}

Heading ExtractHeading(const Vector2f& headingVector)
{
	if (headingVector.m_X < 0.0f)
		return LEFT;
	if (headingVector.m_X > 0.0f)
		return RIGHT;
	if (headingVector.m_Y > 0.0f)
		return UP;

	assert(headingVector.m_Y < 0.0f);
	return DOWN;
}

const Vector2f& ExtractHeadingVector(Heading heading)
{
	static const Vector2f headingVectors[] =
	{
		Vector2f( 0.0f, -1.0f), // DOWN
		Vector2f(-1.0f,  0.0f), // LEFT
		Vector2f( 1.0f,  0.0f), // RIGHT
		Vector2f( 0.0f,  1.0f)  // UP
	};
	return headingVectors[heading];
}

bool Overlap(const MovingEntity& entity1, const MovingEntity& entity2)
{
	static const float allowedOffset = 10.0f;

	Vector2f offsetBetweenCenters = entity1.GetCenter() - entity2.GetCenter();
	offsetBetweenCenters.m_X = std::abs(offsetBetweenCenters.m_X);
	offsetBetweenCenters.m_Y = std::abs(offsetBetweenCenters.m_Y);

	return (offsetBetweenCenters.m_X < allowedOffset) && (offsetBetweenCenters.m_Y < allowedOffset);
}
