#include "pch.h"
#include "GoalLogic.h"
#include "PositionComponent.h"
#include "RadiusComponent.h"
#include "VelocityComponent.h"
#include "GameObject.h"
#include "Vector.h"
#include "Config.h"
#include "EventCenter.h"
#include "PlayGameState.h"

Component::ComponentId GoalLogic::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void GoalLogic::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	GameObject& puck = *gameObjectList[PUCK_ID];

	PositionComponent* goalPositionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	RadiusComponent* goalRadiusComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

	PositionComponent* puckPositionComponent = puck.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* puckVelocityComponent = puck.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);

	const Vector goalPostOffset(0.0f, goalRadiusComponent->GetRadius());
	const Point goalTopPosition = goalPositionComponent->GetCenter() - goalPostOffset;
	const Point goalBottomPosition = goalPositionComponent->GetCenter() + goalPostOffset;

	const Point& puckCenter = puckPositionComponent->GetCenter();
	const Vector& puckVelocity = puckVelocityComponent->Get();

	Ray ray1(goalTopPosition, goalBottomPosition - goalTopPosition);

	const Point oldPuckCenter = puckCenter - deltaTime * puckVelocity;
	Ray ray2(oldPuckCenter, puckCenter - oldPuckCenter);

	float hitParam1, hitParam2;
	bool hitHappened = TestHit(ray1, ray2, hitParam1, hitParam2);

	if (hitHappened && (0.0f <= hitParam1) && (hitParam1 <= 1.0f) && (0.0f <= hitParam2) && (hitParam2 <= 1.0f))
	{
		EventCenter::GetInstance().Send(std::make_unique<Event>(Event::GOAL_SCORED_ID, gameObject.GetId()));
	}
}
