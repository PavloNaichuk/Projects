#pragma once

#include "PhysicsComponent.h"

class PuckPhysics : public PhysicsComponent 
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject) override;
};
