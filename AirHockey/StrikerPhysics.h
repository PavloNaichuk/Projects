#pragma once

#include "PhysicsComponent.h"

class StrikerPhysics : public PhysicsComponent 
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject) override;
};