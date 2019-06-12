#pragma once

#include "Component.h"
#include "Vector.h"

class RadiusComponent : public Component
{
public:
	RadiusComponent(float radius);

	static const ComponentId COMPONENT_ID = 5;
	ComponentId GetId() const override;

	void SetRadius(float radius);
	float GetRadius() const;

private:
	float mRadius;
};
