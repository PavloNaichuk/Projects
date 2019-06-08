#pragma once

#include "Component.h"

class ScoreComponent : public Component
{
public:
	ScoreComponent(unsigned score);

	static const ComponentId COMPONENT_ID = 4;
	ComponentId GetId() const override;

	void Set(unsigned score);
	unsigned Get() const;

private:
	unsigned mScore;
};
