#pragma once

#include "RenderComponent.h"

class EnemyStrikerRenderer : public RenderComponent
{
public:
	ComponentId GetId() const override;
	void Render() override;
};
