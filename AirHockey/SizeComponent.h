#pragma once

#include "Component.h"
#include "Vector.h"

class SizeComponent : public Component
{
public:
	SizeComponent(float radius);

	static const ComponentId COMPONENT_ID = 5;
	ComponentId GetId() const override;

	void SetRadius(float radius);
	float GetRadius() const;

private:
	float mRadius;
};
