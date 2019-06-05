#include "pch.h"
#include "PositionComponent.h"

PositionComponent::PositionComponent(const Point & center, const Vector & velocity, const Size & size)
{
}

void PositionComponent::Set(Vector position)
{
	mPosition = position;
}

Vector PositionComponent::Get()
{
	return mPosition;
}

void PositionComponent::Update(float time)
{
}
