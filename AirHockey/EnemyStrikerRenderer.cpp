#include "pch.h"
#include "EnemyStrikerRenderer.h"
#include "GameObject.h"
#include "PositionComponent.h"
#include "SizeComponent.h"

EnemyStrikerRenderer::EnemyStrikerRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTexture(resourceManager->GetTexture(ResourceManager::EnemyStrikerId))
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
	const SizeComponent* sizeComponent = gameObject.GetComponent<SizeComponent>(SizeComponent::COMPONENT_ID);

	Point topLeft = positionComponent->GetCenter() - 0.5f * sizeComponent->Get();
	SDL_Rect destRect = {int(topLeft.mX), int(topLeft.mY), int(sizeComponent->Get().mX), int(sizeComponent->Get().mY)};

	SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_ADD);
	SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, &destRect);
}
