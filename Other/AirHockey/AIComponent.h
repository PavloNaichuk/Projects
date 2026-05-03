#pragma once

#include "GameObject.h"

class AIComponent : public Component
{
public:
	static const ComponentId COMPONENT_ID = 1;

	virtual ComponentId GetId() const = 0;
	virtual void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) = 0;
};
