#pragma once

#include "Component.h"

class ScoreComponent : public Component
{
public:
	static const ComponentId COMPONENT_ID = 4;
	void Set(unsigned score);
	unsigned Get();

private:
	unsigned mScore;
};
