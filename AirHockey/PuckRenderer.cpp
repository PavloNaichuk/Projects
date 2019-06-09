#include "pch.h"
#include "PuckRenderer.h"
#include "GameObject.h"
#include "PositionComponent.h"
#include "SizeComponent.h"

PuckRenderer::PuckRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTexture(resourceManager->GetTexture(ResourceManager::PUCK_ID))
{
	assert(mTexture);
}

Component::ComponentId PuckRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
}

void PuckRenderer::Render(GameObject& gameObject)
{
	const PositionComponent* positionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	const SizeComponent* sizeComponent = gameObject.GetComponent<SizeComponent>(SizeComponent::COMPONENT_ID);

	Point topLeft = positionComponent->GetCenter() - sizeComponent->GetRadius();
	SDL_Rect destRect = {int(topLeft.mX), int(topLeft.mY), int(2.0f * sizeComponent->GetRadius()), int(2.0f * sizeComponent->GetRadius())};

	SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_ADD);
	SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, &destRect);
}
