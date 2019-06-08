#include "pch.h"
#include "PlayerStrikerMovement.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "GameObject.h"
#include "Common.h"
#include "Config.h"

Component::ComponentId PlayerStrikerMovement::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void PlayerStrikerMovement::Update(GameObject& gameObject, float deltaTime)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	Vector velocity(0, 0);

	if (keyboardState[SDL_SCANCODE_LEFT] == 1)
		velocity.mX = -PLAYER_SPEED;

	if (keyboardState[SDL_SCANCODE_RIGHT] == 1)
		velocity.mX = PLAYER_SPEED;

	if (keyboardState[SDL_SCANCODE_UP] == 1)
		velocity.mY = -PLAYER_SPEED;

	if (keyboardState[SDL_SCANCODE_DOWN] == 1)
		velocity.mY = PLAYER_SPEED;

	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	velocityComponent->Set(velocity);

	PositionComponent* positionComponent = gameObject.GetComponent < PositionComponent>(PositionComponent::COMPONENT_ID);
	const Point& oldCenter = positionComponent->GetCenter();

	Point newCenter = oldCenter + deltaTime * velocity;
	positionComponent->SetCenter(newCenter);
}
