#include "pch.h"
#include "GoalPhysics.h"

Component::ComponentId GoalPhysics::GetId() const 
{
	return PhysicsComponent::COMPONENT_ID;
}

void GoalPhysics::Update(GameObject& gameObject, GameObjectList& gameObjectList)
{
}