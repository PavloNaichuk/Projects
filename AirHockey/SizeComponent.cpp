#include "pch.h"
#include "SizeComponent.h"

SizeComponent::SizeComponent(const Size& size)
	: mSize(size)
{
}

Component::ComponentId SizeComponent::GetId() const 
{
	return COMPONENT_ID;
}

void SizeComponent::Set(const Size& size)
{
	mSize = size;
}

const Size& SizeComponent::Get() const
{
	return mSize;
}
