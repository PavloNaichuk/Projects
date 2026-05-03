#include "MoveState.h"
#include "GameEntities/MovingEntity.h"
#include "CollisionDetection/CollisionDetector.h"

MoveState::MoveState(std::shared_ptr<CollisionDetector> collisionDetector)
	: m_CollisionDetector(std::move(collisionDetector))
{
}

EntityStateType MoveState::GetType() const
{
	return EntityStateType::MOVE;
}

void MoveState::Enter(MovingEntity& entity)
{
}

void MoveState::Update(MovingEntity& entity, std::time_t elapsedTimeInMS)
{
	const Vector2f& entityHeadingVector = ExtractHeadingVector(entity.GetHeading());
	float desiredMoveDist = entity.GetSpeed() * elapsedTimeInMS;
	
	Collision collision;
	m_CollisionDetector->DetectNearestCollision(entity, entityHeadingVector, desiredMoveDist, &collision, CheckVersusStaticEntities);

	float allowedMoveDist = desiredMoveDist - collision.m_Penetration;
	Vector2f moveOffset = allowedMoveDist * entityHeadingVector;
	
	entity.SetCenter(entity.GetCenter() + moveOffset);
}

void MoveState::Exit(MovingEntity& entity)
{
}