#include "pch.h"
#include "PlayerStrikerMovement.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "GameObject.h"
#include "Common.h"

Component::ComponentId PlayerStrikerMovement::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void PlayerStrikerMovement::Update(GameObject& gameObject, float deltaTime)
{
	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	Vector velocity(0, 0);
	if (SDLK_LEFT)
		velocity.mX = -2;
	if (SDLK_RIGHT)
		velocity.mX = 2;
	if (SDLK_UP)
		velocity.mY = -2;
	if (SDLK_DOWN)
		velocity.mY = 2;
	velocityComponent->Set(velocity);

	PositionComponent* positionComponent = gameObject.GetComponent < PositionComponent>(PositionComponent::COMPONENT_ID);
	const Point& oldPosition = positionComponent->GetCenter();
	Point newPosition = oldPosition + deltaTime * velocity;
	positionComponent->SetCenter(newPosition);
}
