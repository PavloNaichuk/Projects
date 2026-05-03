#include "pch.h"
#include "GoalRenderer.h"

Component::ComponentId GoalRenderer::GetId() const 
{
	return RenderComponent::COMPONENT_ID;
}

void GoalRenderer::Render(GameObject& gameObject) 
{
}