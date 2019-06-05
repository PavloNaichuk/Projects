#pragma once

#include "AIComponent.h"

class EnemyStrikerAI : public AIComponent
{
public:
	ComponentId GetId() const override;
	void Update(float deltaTime) override;
};
