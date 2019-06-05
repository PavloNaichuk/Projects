#pragma once

#include "RenderComponent.h"

class PlayerStrikerRenderer : public RenderComponent
{
public:
	ComponentId GetId() const override;
	void Render() override;
};
