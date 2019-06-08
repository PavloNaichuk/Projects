#pragma once

#include "Component.h"
#include "Vector.h"

class SizeComponent : public Component
{
public:
	SizeComponent(const Size& size);

	static const ComponentId COMPONENT_ID = 6;
	ComponentId GetId() const override;

	void Set(const Size& size);
	const Size& Get() const;

private:
	Size mSize;
};
