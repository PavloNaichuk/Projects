#pragma once

#include "RenderComponent.h"
#include "Common.h"
#include "ResourceManager.h"

class GameObject;

class PlayerStrikerRenderer : public RenderComponent
{
public:
	PlayerStrikerRenderer(SharedRenderer renderer, SharedResourceManager resourceManager);

	ComponentId GetId() const override;
	void Render(GameObject& gameObject) override;

private:
	SharedRenderer mRenderer;
	SharedTexture mTexture;
};
