#include "MarioGame.h"

MarioGame::MarioGame()
	: mWindow(nullptr, SDL_DestroyWindow)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return;
	}
	if (TTF_Init() != 0)
	{
		SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
		return;
	}
}

MarioGame::~MarioGame() 
{
	TTF_Quit();
	SDL_Quit();
}

void MarioGame::LaunchGame()
{
	const int windowWidth = 1280;
	const int windowHeight = 720;

	mWindow.reset(SDL_CreateWindow(
		"Mario by Pavlo Naichuk",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	));
	if (mWindow == nullptr)
	{
		SDL_Log("Unable to created window: %s", SDL_GetError());
		return;
	}

	mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE), SDL_DestroyRenderer);
	if (mRenderer == nullptr)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return;
	}
	
	EnterState(std::make_unique<StartMenuState>(*this, mRenderer, windowWidth, windowHeight));

	SDL_Event event;
	for (bool runGame = true; runGame; )
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					runGame = false;
					break;
				}
				case SDL_KEYDOWN:
				{
					mCurrentState->ProcessKeyboard(event.key.keysym.sym);
					break;
				}
			}
		}
		mCurrentState->Update();
		mCurrentState->Render();
	}	
}

void MarioGame::EnterState(std::unique_ptr<GameState> newState)
{
	if (mCurrentState != nullptr)
		mCurrentState->Exit();

	mCurrentState = std::move(newState);
	mCurrentState->Enter();
}

void MarioGame::OnGameStart()
{
	EnterState(std::make_unique<PlayGameState>(*this, mRenderer));
}

void MarioGame::OnLevelLose()
{
}

void MarioGame::OnLevelComplete()
{
}

void MarioGame::OnGameOver()
{
	mCurrentState->Exit();
}
