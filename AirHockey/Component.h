#pragma once

class Component
{
public:
	using ComponentId = unsigned;

	virtual ~Component() {}
	virtual ComponentId GetId() const = 0;
};
