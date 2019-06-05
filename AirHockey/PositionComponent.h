#pragma once

#include "Component.h"
#include "Vector.h"

class PositionComponent : public Component
{
public:
	PositionComponent(const Point& center, const Vector& velocity, const Size& size);
	static const ComponentId COMPONENT_ID = 2;
	void Set(Vector position);
	Vector Get();

private:
	Vector mPosition;
};