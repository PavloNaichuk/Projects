#pragma once

#include "MovementComponent.h"

class GameObject;

class EnemyStrikerMovement : public MovementComponent
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime);
};
