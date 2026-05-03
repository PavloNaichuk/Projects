#pragma once

#define NOMINMAX
#include <windows.h>

#include "Core/MessageCenter.h"
#include <memory>

class GameState;
class RenderSystem;
class Window;

class PacManGame : public MessageListener
{
public:
	PacManGame(HINSTANCE hInstance);

	PacManGame(const PacManGame&) = delete;
	PacManGame& operator= (const PacManGame&) = delete;
	
	int Start(int nCmdShow);

private:
	void EnterGameState(std::shared_ptr<GameState> gameState);
	void OnMessageReceived(const Message& message) override;

private:
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<RenderSystem> m_RenderSystem;
	std::shared_ptr<GameState> m_GameState;
};