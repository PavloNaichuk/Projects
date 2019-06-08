#pragma once

#include "Component.h"

class GameObject;

class RenderComponent : public Component
{
public:
	static const ComponentId COMPONENT_ID = 3;

	virtual ComponentId GetId() const = 0;
	virtual void Render(GameObject& gameObject) = 0;
};
