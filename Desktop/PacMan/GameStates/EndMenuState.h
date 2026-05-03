#pragma once

#include "GameState.h"
#include "Render/RenderSystem.h"
#include <memory>

class RenderSystem;

class EndMenuState : public GameState
{
public:
	EndMenuState(std::shared_ptr<RenderSystem> renderSystem);

	EndMenuState(const EndMenuState&) = delete;
	EndMenuState& operator= (const EndMenuState&) = delete;

	void Enter() override;
	void Update(std::time_t elapsedTimeInMS) override;
	void Exit() override;
	void Render() override;
	void ProcessKeyPress(char key) override;
	void ProcessKeyRelease(char key) override;

private:
	std::shared_ptr<RenderSystem> m_RenderSystem;
	ComPtr<ID2D1SolidColorBrush> m_TextColorBrush;
	ComPtr<IDWriteTextFormat> m_TextFormat;
};