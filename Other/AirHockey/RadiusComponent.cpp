#include "pch.h"
#include "RadiusComponent.h"

RadiusComponent::RadiusComponent(float radius)
	: mRadius(radius)
{
}

Component::ComponentId RadiusComponent::GetId() const 
{
	return COMPONENT_ID;
}

void RadiusComponent::SetRadius(float radius)
{
	mRadius = radius;
}

float RadiusComponent::GetRadius() const
{
	return mRadius;
}
