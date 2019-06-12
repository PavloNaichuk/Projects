#include "pch.h"
#include "StrikerPhysics.h"
#include "RadiusComponent.h"
#include "PositionComponent.h"
#include "GameObject.h"

StrikerPhysics::StrikerPhysics(const Region& movementRegion) 
	: mMovementRegion(movementRegion)
{
}

Component::ComponentId StrikerPhysics::GetId() const 
{
	return PhysicsComponent::COMPONENT_ID;
}

void StrikerPhysics::Update(GameObject& gameObject) 
{
	PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	RadiusComponent* sizeComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

	const float radius = sizeComponent->GetRadius();
	Point center = positionComponent->GetCenter();

	if (center.mX - radius < mMovementRegion.mTopLeft.mX)
		center.mX = mMovementRegion.mTopLeft.mX + radius;
	else if (center.mX + radius > mMovementRegion.mBottonRight.mX)
		center.mX = mMovementRegion.mBottonRight.mX - radius;

	if (center.mY - radius < mMovementRegion.mTopLeft.mY)
		center.mY = mMovementRegion.mTopLeft.mY + radius;
	else if (center.mY + radius > mMovementRegion.mBottonRight.mY)
		center.mY = mMovementRegion.mBottonRight.mY - radius;

	positionComponent->SetCenter(center);


}