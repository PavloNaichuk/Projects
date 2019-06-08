#pragma once

#include "MovementComponent.h"

class GameObject;

class PlayerStrikerMovement : public MovementComponent
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime) override;
};
