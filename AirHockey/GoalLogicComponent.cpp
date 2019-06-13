#include "pch.h"
#include "GoalLogic.h"

Component::ComponentId GoalLogic::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void GoalLogic::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
}
