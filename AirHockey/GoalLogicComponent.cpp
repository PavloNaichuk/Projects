#include "pch.h"
#include "GoalLogicComponent.h"
#include "GameObject.h"

Component::ComponentId GoalLogicComponent::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void GoalLogicComponent::Update(GameObject& gameObject, float deltaTime)
{
}
