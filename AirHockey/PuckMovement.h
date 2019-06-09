#pragma once

#include "MovementComponent.h"

class GameObject;

class PuckMovement : public MovementComponent
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime);
};
