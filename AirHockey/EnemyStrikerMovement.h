#pragma once

#include "AIComponent.h"

class EnemyStrikerMovement : public AIComponent
{
public:
	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) override;
};
