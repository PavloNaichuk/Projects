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

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags)
	{
		SDL_Log("Failed to init required jpg and png support! %s", IMG_GetError());
		return;
	}
}

MarioGame::~MarioGame() 
{
	IMG_Quit();
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
	std::uint32_t prevTime = SDL_GetTicks();
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
					mCurrentState->ProcessKeyPressed(event.key.keysym.sym);
					break;
				}
				case SDL_KEYUP: 
				{
					mCurrentState->ProcessKeyReleased(event.key.keysym.sym);
					break;
				}
			}
		}

		std::uint32_t currTime = SDL_GetTicks();
		float elapsedTime = float(currTime - prevTime) / 1000.0f;
		prevTime = currTime;

		mCurrentState->Update(elapsedTime);
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
	EnterState(std::make_unique<EndMenuState>(*this, mRenderer, WINDOW_WIDTH, WINDOW_HEIGHT));
}

void MarioGame::OnLevelComplete()
{
}

void MarioGame::OnGameOver()
{
	mCurrentState->Exit();
}
