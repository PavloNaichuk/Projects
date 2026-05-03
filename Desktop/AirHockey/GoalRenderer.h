#pragma once

#include "RenderComponent.h"

class GoalRenderer : public RenderComponent 
{
public:
	ComponentId GetId() const override;
	void Render(GameObject& gameObject) override;
};

