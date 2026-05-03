#include "EndMenuState.h"
#include "Render/RenderSystem.h"
#include "Core/Color.h"

EndMenuState::EndMenuState(std::shared_ptr<RenderSystem> renderSystem)
	: m_RenderSystem(renderSystem)
{
}

void EndMenuState::Enter()
{
	m_TextColorBrush = m_RenderSystem->CreateTextColorBrush(Color::BLUE);

	m_TextFormat = m_RenderSystem->CreateTextFormat(L"Consolas", 40.0f);
	m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void EndMenuState::Update(std::time_t /*elapsedTimeInMS*/)
{
}

void EndMenuState::Exit()
{
	m_TextColorBrush.Reset();
	m_TextFormat.Reset();
}

void EndMenuState::Render()
{
	m_RenderSystem->BeginRenderFrame();
	m_RenderSystem->ClearRenderTarget(Color::BLACK);

	m_RenderSystem->DrawText(L"Game Over", m_TextFormat, m_TextColorBrush,
		 0.0f,
		 0.0f,
		(FLOAT)m_RenderSystem->GetBackBufferWidth(),
		(FLOAT)m_RenderSystem->GetBackBufferHeight());

	m_RenderSystem->EndRenderFrame();
}

void EndMenuState::ProcessKeyPress(char key)
{
}

void EndMenuState::ProcessKeyRelease(char key)
{
}
