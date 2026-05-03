#pragma once

#include "Common.h"

class Component
{
public:
	using ComponentId = unsigned;

	virtual ~Component() {}
	virtual ComponentId GetId() const = 0;
};

using UniqueComponent = std::unique_ptr<Component>;