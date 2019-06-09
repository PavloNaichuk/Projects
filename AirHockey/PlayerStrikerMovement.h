#pragma once

#include "AIComponent.h"

class GameObject;

class PlayerStrikerMovement : public AIComponent
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime) override;
};
