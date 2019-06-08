#include "pch.h"
#include "VelocityComponent.h"

VelocityComponent::VelocityComponent(const Vector& velocity)
	: mVelocity(velocity)
{
}

Component::ComponentId VelocityComponent::GetId() const 
{
	return COMPONENT_ID;
}

void VelocityComponent::Set(const Vector& velocity)
{
	mVelocity = velocity;
}

const Vector& VelocityComponent::Get() const
{
	return mVelocity;
}
