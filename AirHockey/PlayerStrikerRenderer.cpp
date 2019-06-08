#include "pch.h"
#include "PlayerStrikerRenderer.h"
#include "GameObject.h"
#include "PositionComponent.h"
#include "SizeComponent.h"

PlayerStrikerRenderer::PlayerStrikerRenderer(SharedRenderer renderer, SharedResourceManager resourceManager) 
	: mRenderer(renderer)
	, mTexture(resourceManager->GetTexture(ResourceManager::PlayerStrikerId))
{
	assert(mTexture);
}

Component::ComponentId PlayerStrikerRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
}

void PlayerStrikerRenderer::Render(GameObject& gameObject)
{
	const PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	const SizeComponent* sizeComponent = gameObject.GetComponent<SizeComponent>(SizeComponent::COMPONENT_ID);

	Point topLeft = positionComponent->GetCenter() - 0.5f * sizeComponent->Get();
	SDL_Rect destRect = {int(topLeft.mX), int(topLeft.mY), int(sizeComponent->Get().mX), int(sizeComponent->Get().mY)};

	SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_ADD);
	SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, &destRect);
}
