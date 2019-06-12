#include "pch.h"
#include "PuckPhysics.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "RadiusComponent.h"
#include "GameObject.h"

PuckPhysics::PuckPhysics(std::vector<BoardWall> boardWalls) 
	: mBoardWalls(std::move(boardWalls))
{
}

Component::ComponentId PuckPhysics::GetId() const 
{
	return PuckPhysics::COMPONENT_ID;
}

void PuckPhysics::Update(GameObject& gameObject) 
{
	PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* velocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	RadiusComponent* sizeComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);



}