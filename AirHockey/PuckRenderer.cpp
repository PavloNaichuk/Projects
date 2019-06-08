#include "pch.h"
#include "PuckRenderer.h"
#include "GameObject.h"
#include "PositionComponent.h"
#include "SizeComponent.h"

PuckRenderer::PuckRenderer(SharedRendererPointer renderer, SharedResourceManagerPointer resourceManager)
	: mRenderer(renderer)
	, mTexture(resourceManager->GetTexture(ResourceManager::Puck))
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

	Point topLeft = positionComponent->GetCenter() - 0.5f * sizeComponent->Get();
	SDL_Rect destRect = {int(topLeft.mX), int(topLeft.mY), int(sizeComponent->Get().mX), int(sizeComponent->Get().mY)};

	SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, &destRect);
}
