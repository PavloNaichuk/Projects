#pragma once

#include "PhysicsComponent.h"
#include "Helpers.h"

class StrikerPhysics : public PhysicsComponent 
{
public:
	StrikerPhysics(const Region& movementRegion);

	ComponentId GetId() const override;
	void Update(GameObject& gameObject) override;

private:
	Region mMovementRegion;
};