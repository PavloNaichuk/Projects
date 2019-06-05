#pragma once
#include "AIComponent.h"

class PuckAI : public AIComponent
{
public:
	ComponentId GetId() const override;
	void Update(float deltaTime) override;
};
