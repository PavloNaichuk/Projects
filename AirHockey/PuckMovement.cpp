#include "pch.h"
#include "PuckMovement.h"
#include "GameObject.h"

Component::ComponentId PuckMovement::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void PuckMovement::Update(GameObject& gameObject, float deltaTime)
{
}