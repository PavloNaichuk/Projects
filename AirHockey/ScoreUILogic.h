#pragma once

#include "AIComponent.h"

class ScoreUILogic : public AIComponent
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime) override;
};