#include "pch.h"
#include "AirHockey.h"
#include "PlayGameState.h"

AirHockey::AirHockey()
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

AirHockey::~AirHockey()
{
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void AirHockey::LaunchGame()
{
	const int windowWidth = 1280;
	const int windowHeight = 720;

	mWindow.reset(SDL_CreateWindow(
		"AirHockey by Pavlo Naichuk",
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

	EnterState(std::make_unique<PlayGameState>(mRenderer, mResourceManager));

	SDL_Event event;
	std::uint32_t prevTime = SDL_GetTicks();
	while (true)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break;
		}

		std::uint32_t currTime = SDL_GetTicks();
		float elapsedTime = float(currTime - prevTime) / 1000.0f;
		prevTime = currTime;

		mCurrentState->Update(elapsedTime);
		mCurrentState->Render();
	}
}

void AirHockey::EnterState(std::unique_ptr<GameState> newState)
{
	if (mCurrentState != nullptr)
		mCurrentState->Exit();

	mCurrentState = std::move(newState);
	mCurrentState->Enter();
}
