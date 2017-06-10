#include "StartMenuState.h"
#include "Core/MessageCenter.h"
#include "Core/Color.h"

StartMenuState::StartMenuState(std::shared_ptr<RenderSystem> renderSystem)
	: m_RenderSystem(renderSystem)
{
}

void StartMenuState::Enter()
{
	m_TextColorBrush = m_RenderSystem->CreateTextColorBrush(Color::BLUE);

	m_TitleTextFormat = m_RenderSystem->CreateTextFormat(L"Consolas", 80.0f);
	m_TitleTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_TitleTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	m_ControlsTextFormat = m_RenderSystem->CreateTextFormat(L"Consolas", 20.0f);
	m_ControlsTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_ControlsTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		
	m_StartTextFormat = m_RenderSystem->CreateTextFormat(L"Consolas", 20.0f);
	m_StartTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_StartTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void StartMenuState::Update(std::time_t /*elapsedTimeInMS*/)
{
}

void StartMenuState::Exit()
{
	m_TextColorBrush.Reset();
	m_TitleTextFormat.Reset();
	m_ControlsTextFormat.Reset();
	m_StartTextFormat.Reset();
}

void StartMenuState::Render()
{
	m_RenderSystem->BeginRenderFrame();
	m_RenderSystem->ClearRenderTarget(Color::BLACK);
	
	{
		FLOAT textRectBottom = (FLOAT)m_RenderSystem->GetBackBufferHeight() / 3.0f;
		
		m_RenderSystem->DrawText(L"Pac-Man", m_TitleTextFormat, m_TextColorBrush,
			0.0f, 0.0f, (FLOAT)m_RenderSystem->GetBackBufferWidth(), textRectBottom);
	}
	{
		FLOAT textRectTop = (FLOAT)m_RenderSystem->GetBackBufferHeight() / 3.0f;
		FLOAT textRectBottom = 2.0f * textRectTop;

		m_RenderSystem->DrawText(L"Use keys Up, Down, Left, Right to navigate", m_ControlsTextFormat, m_TextColorBrush,
			0.0f, textRectTop, (FLOAT)m_RenderSystem->GetBackBufferWidth(), textRectBottom);
	}
	{
		FLOAT textRectBottom = (FLOAT)m_RenderSystem->GetBackBufferHeight();
		FLOAT textRectTop = textRectBottom - (FLOAT)m_RenderSystem->GetBackBufferHeight() / 3.0f;

		m_RenderSystem->DrawText(L"Press Enter to start", m_StartTextFormat, m_TextColorBrush,
			0.0f, textRectTop, (FLOAT)m_RenderSystem->GetBackBufferWidth(), textRectBottom);
	}
		
	m_RenderSystem->EndRenderFrame();
}

void StartMenuState::ProcessKeyPress(char key)
{
	if (key == VK_RETURN)
	{
		MessageCenter::GetInstance().AddMessage({EnterGameRequested, this, nullptr});
	}
}

void StartMenuState::ProcessKeyRelease(char key)
{
}
