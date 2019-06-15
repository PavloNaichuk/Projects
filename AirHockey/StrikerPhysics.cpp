#include "pch.h"
#include "StrikerPhysics.h"
#include "RadiusComponent.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "GameObject.h"

StrikerPhysics::StrikerPhysics(const Region& movementRegion) 
	: mMovementRegion(movementRegion)
{
}

Component::ComponentId StrikerPhysics::GetId() const 
{
	return PhysicsComponent::COMPONENT_ID;
}

void StrikerPhysics::Update(GameObject& gameObject, GameObjectList& gameObjectList)
{
	GameObject& puck = *gameObjectList[PUCK_ID];

	PositionComponent* strikerPositionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	RadiusComponent* strikerRadiusComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);
		
	PositionComponent* puckPositionComponent = puck.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* puckVelocityComponent = puck.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	RadiusComponent* puckRadiusComponent = puck.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);
		
	Point strikerCenter = strikerPositionComponent->GetCenter();
	float strikerRadius = strikerRadiusComponent->GetRadius();

	Point puckCenter = puckPositionComponent->GetCenter();
	float puckRadius = puckRadiusComponent->GetRadius();

	if (strikerCenter.mX - strikerRadius < mMovementRegion.mTopLeft.mX)
		strikerCenter.mX = mMovementRegion.mTopLeft.mX + strikerRadius;
	else if (strikerCenter.mX + strikerRadius > mMovementRegion.mBottonRight.mX)
		strikerCenter.mX = mMovementRegion.mBottonRight.mX - strikerRadius;

	if (strikerCenter.mY - strikerRadius < mMovementRegion.mTopLeft.mY)
		strikerCenter.mY = mMovementRegion.mTopLeft.mY + strikerRadius;
	else if (strikerCenter.mY + strikerRadius > mMovementRegion.mBottonRight.mY)
		strikerCenter.mY = mMovementRegion.mBottonRight.mY - strikerRadius;

	strikerPositionComponent->SetCenter(strikerCenter);

	if (CirclesHitEachOther(strikerCenter, strikerRadius, puckCenter, puckRadius))
	{
		Vector dirBetweenCenters = Normalize(puckCenter - strikerCenter);

		Point puckNewCenter = strikerCenter + (strikerRadius + puckRadius) * dirBetweenCenters;
		puckPositionComponent->SetCenter(puckNewCenter);

		puckVelocityComponent->Set(Vector(0.0f, 0.0f));
	}
}