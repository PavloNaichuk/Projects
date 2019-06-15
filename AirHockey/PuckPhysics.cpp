#include "pch.h"
#include "PuckPhysics.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "RadiusComponent.h"
#include "EventCenter.h"
#include "GameObject.h"
#include "Config.h"

PuckPhysics::PuckPhysics(std::vector<BoardWall> boardWalls) 
	: mBoardWalls(std::move(boardWalls))
{
}

Component::ComponentId PuckPhysics::GetId() const
{
	return PuckPhysics::COMPONENT_ID;
}

void PuckPhysics::Update(GameObject& gameObject, GameObjectList& gameObjectList)
{
	PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	RadiusComponent* radiusComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

	const Point& center = positionComponent->GetCenter();
	const Vector& velocity = velocityComponent->Get();
	const float radius = radiusComponent->GetRadius();
	
	const BoardWall* collisionBoardWall = nullptr;
	float largestPenetrationSquaredDist = 0.0f;
	Point outmostCollisionPoint;

	for (const BoardWall& boardWall : mBoardWalls)
	{
		Point outmostPoint = center - radius * boardWall.mOrientation;
		
		Vector dirToWall = boardWall.mStart - outmostPoint;
		bool isBehindTheWall = Dot(dirToWall, boardWall.mOrientation) > 0.0f;

		if (isBehindTheWall)
		{
			Ray ray1(outmostPoint, -velocity);
			Ray ray2(boardWall.mStart, boardWall.mEnd - boardWall.mStart);

			float hitParam1, hitParam2;
			bool hitBetweenRaysHappened = TestHit(ray1, ray2, hitParam1, hitParam2);
			
			bool collisionHappened = (0.0f <= hitParam1) && (hitParam1 <= 1.0f) && (0.0f <= hitParam2) && (hitParam2 <= 1.0f);
			if (collisionHappened)
			{
				Point collisionPoint = CalcPointOnRay(ray1, hitParam1);

				float penetrationSquaredDist = SquaredDistance(collisionPoint, outmostPoint);
				if (penetrationSquaredDist > largestPenetrationSquaredDist)
				{
					collisionBoardWall = &boardWall;
					largestPenetrationSquaredDist = penetrationSquaredDist;
					outmostCollisionPoint = collisionPoint;
				}
			}
		}
	}

	if (collisionBoardWall != nullptr)
	{
		const Point newCenter = outmostCollisionPoint + radius * collisionBoardWall->mOrientation;
		positionComponent->SetCenter(newCenter);

		const Vector newVelocity = BOARD_WALL_RESTITUTION * Reflect(velocity, collisionBoardWall->mOrientation);
		velocityComponent->Set(newVelocity);

		EventCenter::GetInstance().Send(std::make_unique<Event>(Event::PUCK_BOARD_WALL_HIT_ID, gameObject.GetId()));
	}
}