#include "pch.h"
#include "EnemyStrikerRenderer.h"
#include "GameObject.h"
#include "PositionComponent.h"
#include "RadiusComponent.h"

EnemyStrikerRenderer::EnemyStrikerRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTexture(resourceManager->GetTexture(ResourceManager::ENEMY_STRIKER_ID))
{
	assert(mTexture);
}

Component::ComponentId EnemyStrikerRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
}

void EnemyStrikerRenderer::Render(GameObject& gameObject)
{
	const PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	const RadiusComponent* radiusComponent = gameObject.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

	Point topLeft = positionComponent->GetCenter() - radiusComponent->GetRadius();
	SDL_Rect destRect = {int(topLeft.mX), int(topLeft.mY), int(2.0f * radiusComponent->GetRadius()), int(2.0f * radiusComponent->GetRadius())};

	SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_ADD);
	SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, &destRect);
}
