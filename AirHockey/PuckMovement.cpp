#include "pch.h"
#include "PuckMovement.h"
#include "GameObject.h"
#include "Config.h"
#include "Vector.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"

Component::ComponentId PuckMovement::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void PuckMovement::Update(GameObject& gameObject, float deltaTime)
{
	Vector velocity(0, 0);
	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	velocityComponent->Set(velocity);

	PositionComponent* positionComponent = gameObject.GetComponent < PositionComponent>(PositionComponent::COMPONENT_ID);
	const Point& oldCenter = positionComponent->GetCenter();

	Point newCenter = oldCenter + deltaTime * velocity;
	positionComponent->SetCenter(newCenter);
}