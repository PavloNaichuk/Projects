#include "pch.h"
#include "SizeComponent.h"

SizeComponent::SizeComponent(float radius)
	: mRadius(radius)
{
}

Component::ComponentId SizeComponent::GetId() const 
{
	return COMPONENT_ID;
}

void SizeComponent::SetRadius(float radius)
{
	mRadius = radius;
}

float SizeComponent::GetRadius() const
{
	return mRadius;
}
