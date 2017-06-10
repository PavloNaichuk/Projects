#include "PacManGame.h"
#include "Core/Config.h"
#include "Core/Window.h"
#include "Core/LevelMap.h"
#include "GameStates/StartMenuState.h"
#include "GameStates/PlayGameState.h"
#include "GameStates/EndMenuState.h"
#include "Render/RenderSystem.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

PacManGame::PacManGame(HINSTANCE hInstance)
{
	const LONG windowX = (GetSystemMetrics(SM_CXSCREEN) - Config::WINDOW_WIDTH) / 2;
	const LONG windowY = (GetSystemMetrics(SM_CYSCREEN) - Config::WINDOW_HEIGHT) / 2;
	
	m_Window = std::make_shared<Window>(hInstance, WndProc, "PacManWindow", "Pac-Man by Pavlo Naichuk", windowX, windowY, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
	m_RenderSystem = std::make_shared<RenderSystem>(m_Window);
}

int PacManGame::Start(int nCmdShow)
{
	MessageCenter::GetInstance().RegisterListener(this);

	auto startMenuState = std::make_shared<StartMenuState>(m_RenderSystem);
	EnterGameState(startMenuState);
	
	m_Window->Show(nCmdShow);

	std::time_t lastTimeInMS = GetTickCount();

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
			else if (msg.message == WM_KEYDOWN)
			{
				m_GameState->ProcessKeyPress(msg.wParam);
			}
			else if (msg.message == WM_KEYUP)
			{
				m_GameState->ProcessKeyRelease(msg.wParam);
			}
		}
		MessageCenter::GetInstance().ProcessMessages();
		
		std::time_t currTimeInMS = GetTickCount();
		std::time_t elapsedTimeInMS = currTimeInMS - lastTimeInMS;
		lastTimeInMS = currTimeInMS;

		m_GameState->Update(elapsedTimeInMS);
		m_GameState->Render();
	}

	MessageCenter::GetInstance().UnregisterListener(this);
	return msg.wParam;
}

void PacManGame::EnterGameState(std::shared_ptr<GameState> gameState)
{
	if (m_GameState)
		m_GameState->Exit();
	
	m_GameState = gameState;
	m_GameState->Enter();
}

void PacManGame::OnMessageReceived(const Message& message)
{
	if (message.m_ID == EnterGameRequested)
	{
		const char* pathToLevelMap = "Maps/Map1.txt";
		auto levelMap = std::make_shared<LevelMap>(pathToLevelMap);

		auto playGameState = std::make_shared<PlayGameState>(levelMap, m_RenderSystem);
		EnterGameState(playGameState);

		return;
	}
	if (message.m_ID == LevelCompleted)
	{
		auto endMenuState = std::make_shared<EndMenuState>(m_RenderSystem);
		EnterGameState(endMenuState);

		return;
	}
	if (message.m_ID == LevelFailed)
	{
		auto endMenuState = std::make_shared<EndMenuState>(m_RenderSystem);
		EnterGameState(endMenuState);

		return;
	}
}