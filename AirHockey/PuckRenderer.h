#pragma once
#include "RenderComponent.h"

class PuckRenderer : public RenderComponent
{
public:
	ComponentId GetId() const override;
	void Render() override;
};
