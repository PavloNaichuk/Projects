#pragma once

#include "GameObject.h"

class PhysicsComponent : public Component
{
public:
	static const ComponentId COMPONENT_ID = 2;

	virtual ComponentId GetId() const = 0;
	virtual void Update(GameObject& gameObject, GameObjectList& gameObjectList) = 0;
};