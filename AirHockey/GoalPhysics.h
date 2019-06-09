#pragma once

#include "PhysicsComponent.h"

class GoalPhysics : public PhysicsComponent 
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject) override;
};
