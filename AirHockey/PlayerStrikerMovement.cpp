#include "pch.h"
#include "PlayerStrikerMovement.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "RadiusComponent.h"
#include "GameObject.h"
#include "Common.h"
#include "Config.h"

Component::ComponentId PlayerStrikerMovement::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void PlayerStrikerMovement::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	PositionComponent* strikerPositionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* strikerVelocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
			
	Vector strikerVelocity(0, 0);
	
	if (keyboardState[SDL_SCANCODE_LEFT] == 1)
		strikerVelocity.mX = -STRIKER_SPEED;
	
	if (keyboardState[SDL_SCANCODE_RIGHT] == 1)
		strikerVelocity.mX = STRIKER_SPEED;
	
	if (keyboardState[SDL_SCANCODE_UP] == 1)
		strikerVelocity.mY = -STRIKER_SPEED;
	
	if (keyboardState[SDL_SCANCODE_DOWN] == 1)
		strikerVelocity.mY = STRIKER_SPEED;

	strikerVelocityComponent->Set(strikerVelocity);

	Point strikerNewCenter = strikerPositionComponent->GetCenter() + deltaTime * strikerVelocity;
	strikerPositionComponent->SetCenter(strikerNewCenter);
		
	if (keyboardState[SDL_SCANCODE_LCTRL] == 1)
	{
		GameObject& puck = *gameObjectList[PUCK_ID];

		PositionComponent* puckPositionComponent = puck.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
		VelocityComponent* puckVelocityComponent = puck.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
								
		RadiusComponent* strikerRadiusComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);
		RadiusComponent* puckRadiusComponent = puck.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

		const Point& puckCenter = puckPositionComponent->GetCenter();
		float strikerRadius = strikerRadiusComponent->GetRadius();
		float puckRadius = puckRadiusComponent->GetRadius();
		
		if (CirclesHitEachOther(strikerNewCenter, strikerRadius, puckCenter, puckRadius))
		{
			Vector puckVelocity = PUCK_SPEED * Normalize(puckCenter - strikerNewCenter);
			puckVelocityComponent->Set(puckVelocity);
		}
	}
}
