#pragma once

#include "Component.h"
#include "Vector.h"

class VelocityComponent : public Component
{
public:
	VelocityComponent(const Vector& velocity);

	static const ComponentId COMPONENT_ID = 5;
	ComponentId GetId() const override;

	void Set(const Vector& velocity);
	const Vector& Get() const;

private:
	Vector mVelocity;
};
