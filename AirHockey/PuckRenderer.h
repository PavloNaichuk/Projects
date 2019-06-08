#pragma once

#include "RenderComponent.h"
#include "Common.h"
#include "ResourceManager.h"

class GameObject;

class PuckRenderer : public RenderComponent
{
public:
	PuckRenderer(SharedRendererPointer renderer, SharedResourceManagerPointer resourceManager);

	ComponentId GetId() const override;
	void Render(GameObject& gameObject) override;

private:
	SharedRendererPointer mRenderer;
	SharedTexturePointer mTexture;
};
