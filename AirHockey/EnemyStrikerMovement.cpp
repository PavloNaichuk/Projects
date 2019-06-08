#include "pch.h"
#include "EnemyStrikerMovement.h"
#include "GameObject.h"

Component::ComponentId EnemyStrikerMovement::GetId() const
{
	return MovementComponent::COMPONENT_ID;
}

void EnemyStrikerMovement::Update(GameObject& gameObject, float deltaTime)
{
}
