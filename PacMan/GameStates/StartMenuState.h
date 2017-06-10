#pragma once

#include "GameState.h"
#include "Render/RenderSystem.h"
#include <memory>

class StartMenuState : public GameState
{
public:
	StartMenuState(std::shared_ptr<RenderSystem> renderSystem);

	StartMenuState(const StartMenuState&) = delete;
	StartMenuState& operator= (const StartMenuState&) = delete;

	void Enter() override;
	void Update(std::time_t elapsedTimeInMS) override;
	void Exit() override;
	void Render() override;
	void ProcessKeyPress(char key) override;
	void ProcessKeyRelease(char key) override;

private:
	std::shared_ptr<RenderSystem> m_RenderSystem;
	ComPtr<ID2D1SolidColorBrush> m_TextColorBrush;
	ComPtr<IDWriteTextFormat> m_TitleTextFormat;
	ComPtr<IDWriteTextFormat> m_ControlsTextFormat;
	ComPtr<IDWriteTextFormat> m_StartTextFormat;
};