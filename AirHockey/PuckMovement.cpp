#include "pch.h"
#include "PuckMovement.h"
#include "GameObject.h"
#include "Config.h"
#include "Vector.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"

Component::ComponentId PuckMovement::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void PuckMovement::Update(GameObject& gameObject, float deltaTime)
{
	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);

	Vector velocity = (1.0f - BOARD_FRICTION) * velocityComponent->Get();
	velocityComponent->Set(velocity);
	
	const Point& oldCenter = positionComponent->GetCenter();
	Point newCenter = oldCenter + deltaTime * velocity;

	positionComponent->SetCenter(newCenter);
}