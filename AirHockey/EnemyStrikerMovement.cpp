#include "pch.h"
#include "EnemyStrikerMovement.h"
#include "GameObject.h"
#include "Config.h"
#include "Vector.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"

Component::ComponentId EnemyStrikerMovement::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void EnemyStrikerMovement::Update(GameObject& gameObject, float deltaTime)
{
	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);

	Vector velocity(0, 0);

	velocityComponent->Set(velocity);

	const Point& oldCenter = positionComponent->GetCenter();

	Point newCenter = oldCenter + deltaTime * velocity;
	positionComponent->SetCenter(newCenter);
}
