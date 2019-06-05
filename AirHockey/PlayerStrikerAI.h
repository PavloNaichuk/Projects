#pragma once

#include "AIComponent.h"

class PlayerStrikerAI : public AIComponent
{
public:
	ComponentId GetId() const override;
	void Update(float deltaTime) override;
};
