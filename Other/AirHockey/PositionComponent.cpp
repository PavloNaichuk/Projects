#include "pch.h"
#include "PositionComponent.h"

PositionComponent::PositionComponent(const Point& center)
	: mCenter(center)
{
}

Component::ComponentId PositionComponent::GetId() const 
{
	return COMPONENT_ID;
}

void PositionComponent::SetCenter(const Point& center)
{
	mCenter = center;
}

const Point& PositionComponent::GetCenter() const
{
	return mCenter;
}


