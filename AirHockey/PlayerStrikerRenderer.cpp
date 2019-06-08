#include "pch.h"
#include "PlayerStrikerRenderer.h"
#include "GameObject.h"
#include "PositionComponent.h"
#include "SizeComponent.h"

PlayerStrikerRenderer::PlayerStrikerRenderer(SharedRendererPointer renderer, SharedResourceManagerPointer resourceManager) 
	: mRenderer(renderer)
	, mTexture(resourceManager->GetTexture(ResourceManager::PlayerStriker))
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

	SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, &destRect);
}
