#pragma once

#include "Component.h"

class RenderComponent : public Component
{
public:
	static const ComponentId COMPONENT_ID = 3;

	virtual ComponentId GetId() const = 0;
	virtual void Render() = 0;
};
