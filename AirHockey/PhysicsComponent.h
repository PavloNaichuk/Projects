#pragma once

#include "Component.h"

class GameObject;

class PhysicsComponent : public Component
{
public:
	static const ComponentId COMPONENT_ID = 2;

	virtual ComponentId GetId() const = 0;
	virtual void Update(GameObject& gameObject) = 0;
};