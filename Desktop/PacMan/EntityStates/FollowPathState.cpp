#include "FollowPathState.h"
#include "PathFinding/Path.h"
#include "PathFinding/NavigationGraph.h"
#include "GameEntities/MovingEntity.h"
#include "CollisionDetection/CollisionDetector.h"
#include "Core/LevelMap.h"
#include <algorithm>

FollowPathState::FollowPathState(std::shared_ptr<CollisionDetector> collisionDetector)
	: m_CollisionDetector(std::move(collisionDetector))
{
}

EntityStateType FollowPathState::GetType() const
{
	return EntityStateType::FOLLOW_PATH;
}

void FollowPathState::Enter(MovingEntity& entity)
{
}

void FollowPathState::Update(MovingEntity& entity, std::time_t elapsedTimeInMS)
{
	auto path = entity.GetPath();
	if (path->ReachedEnd())
		return;
	
	const Vector2f& entityCenter = entity.GetCenter();
	
	const GraphNode* pGraphNode = path->GetCurrentNode();
	const Vector2f checkPoint = CalcMapTileCenter(pGraphNode->m_TileRow, pGraphNode->m_TileCol);

	Vector2f entityHeadingVector = checkPoint - entityCenter;
	float distToCheckPoint = Length(entityHeadingVector);

	if (distToCheckPoint > 0.0f)
	{
		entityHeadingVector = Normalize(entityHeadingVector);
				
		float desiredMoveDist = entity.GetSpeed() * elapsedTimeInMS;
		desiredMoveDist = std::min(desiredMoveDist, distToCheckPoint);

		Collision collision;
		m_CollisionDetector->DetectNearestCollision(entity, entityHeadingVector, desiredMoveDist, &collision, CheckVersusGhosts);

		float allowedMoveDist = desiredMoveDist - collision.m_Penetration;
		Vector2f moveOffset = allowedMoveDist * entityHeadingVector;

		entity.SetHeading(ExtractHeading(entityHeadingVector));
		entity.SetCenter(entityCenter + moveOffset);
	}
	else
	{
		path->SetNextNode();
	}
}

void FollowPathState::Exit(MovingEntity& entity)
{
}