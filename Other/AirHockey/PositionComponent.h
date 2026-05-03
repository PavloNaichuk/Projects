#pragma once

#include "Component.h"
#include "Math.h"

class PositionComponent : public Component
{
public:
	PositionComponent(const Point& center);

	static const ComponentId COMPONENT_ID = 4;

	ComponentId GetId() const override;

	void SetCenter(const Point& center);
	const Point& GetCenter() const;

private:
	Point mCenter;
};