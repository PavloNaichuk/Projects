#include "pch.h"
#include "AirHockey.h"
#include "PlayGameState.h"
#include "EndMenuState.h"
#include "EventCenter.h"
#include "Config.h"

int AirHockey::LaunchGame()
{
	int statusCode = Init();
	if (statusCode != 0)
		return statusCode;

	GameLoop();

	return Deinit();
}

int AirHockey::Init() 
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		assert(false);
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}
	if (TTF_Init() != 0)
	{
		assert(false);
		SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
		return 2;
	}
	
	const int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags)
	{
		assert(false);
		SDL_Log("Failed to init required jpg and png support: %s", IMG_GetError());
		return 4;
	}

	mWindow.reset(SDL_CreateWindow("Air Hockey by Pavlo Naichuk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		BOARD_WIDTH, UI_AREA_HEIGHT + BOARD_HEIGHT, SDL_WINDOW_OPENGL), SDL_DestroyWindow);

	if (mWindow == nullptr)
	{
		assert(false);
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return 5;
	}

	mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
	if (mRenderer == nullptr)
	{
		assert(false);
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return 6;
	}

	mResourceManager.reset(new ResourceManager());
	if (mResourceManager->LoadResources(mRenderer) == false) 
	{
		assert(false);
		SDL_Log("Unable to load resources");
		return 7;
	}

	return 0;
}

int AirHockey::Deinit()
{
	mCurrentState.reset();
	mResourceManager.reset();
	mRenderer.reset();
	mWindow.reset();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}

void AirHockey::GameLoop() 
{
	auto handleEvent = [this](const Event& event) 
	{
		if (event.mEventId == Event::PLAY_TIME_FINISHED_ID) 
		{
			EnterState(std::make_unique<EndMenuState>(mRenderer, BOARD_WIDTH, BOARD_HEIGHT));
		}
	};
	EventCenter::GetInstance().Subscribe(handleEvent);

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

		EventCenter::GetInstance().Update();
	}
}


void AirHockey::EnterState(UniqueGameState newState)
{
	if (mCurrentState != nullptr)
		mCurrentState->Exit();

	mCurrentState = std::move(newState);
	mCurrentState->Enter();
}
