#include "pch.h"
#include "BoxComponent.h"

BoxComponent::BoxComponent(const Point& topLeft, int width, int height) 
	: mTopLeft(topLeft)
	, mWidth(width)
	, mHeight(height)
{
}

Component::ComponentId BoxComponent::GetId() const 
{
	return COMPONENT_ID;
}

const Point& BoxComponent::GetTopLeft() const 
{
	return mTopLeft;
}

void BoxComponent::SetTopLeft(const Point& topLeft) 
{
	mTopLeft = topLeft;
}

int BoxComponent::GetWidth() const 
{
	return mWidth;
}

void BoxComponent::SetWidth(int width) 
{
	mWidth = width;
}

int BoxComponent::GetHeight() const 
{
	return mHeight;
}

void BoxComponent::SetHeight(int height) 
{
	mHeight = height;
}