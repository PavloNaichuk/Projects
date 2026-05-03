#pragma once

#include "Component.h"
#include "Math.h"

class BoxComponent : public Component 
{
public:
	BoxComponent(const Point& topLeft, int width, int height);

	static const ComponentId COMPONENT_ID = 8;

	ComponentId GetId() const override;

	const Point& GetTopLeft() const;
	void SetTopLeft(const Point& topLeft);

	int GetWidth() const;
	void SetWidth(int width);

	int GetHeight() const;
	void SetHeight(int height);

private:
	Point mTopLeft;
	int mWidth;
	int mHeight;
};