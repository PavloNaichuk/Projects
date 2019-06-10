#pragma once

#include "RenderComponent.h"
#include "ResourceManager.h"

class ScoreUIRenderer : public RenderComponent
{
public:
	ScoreUIRenderer(SharedRenderer renderer, SharedResourceManager resourceManager);

	ComponentId GetId() const override;
	void Render(GameObject& gameObject) override;

private:
	SharedRenderer mRenderer;
	SharedFont mTextFont;
	SharedSurface mTextSurface;
	SharedTexture mTextTexture;

	int mPrevScore;
};
