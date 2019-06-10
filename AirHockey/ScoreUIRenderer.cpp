#include "pch.h"
#include "ScoreUIRenderer.h"

ScoreUIRenderer::ScoreUIRenderer(SharedRenderer renderer, SharedResourceManager resourceManager)
	: mRenderer(renderer)
	, mTextFont(resourceManager->GetTextFont(ResourceManager::SCORE_TEXT_ID))
{
}

Component::ComponentId ScoreUIRenderer::GetId() const
{
	return RenderComponent::COMPONENT_ID;
};

void ScoreUIRenderer::Render(GameObject& gameObject) 
{
}