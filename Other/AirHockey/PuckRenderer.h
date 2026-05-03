#pragma once

#include "RenderComponent.h"
#include "Common.h"
#include "ResourceManager.h"

class PuckRenderer : public RenderComponent
{
public:
	PuckRenderer(SharedRenderer renderer, SharedResourceManager resourceManager);

	ComponentId GetId() const override;
	void Render(GameObject& gameObject) override;

private:
	SharedRenderer mRenderer;
	SharedTexture mTexture;
};
