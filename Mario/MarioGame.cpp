#include "MarioGame.h"

void MarioGame::LaunchGame()
{
	while (true) 
	{
		const int windowWidth = 1280;
		const int windowHeight = 720;

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return 1;
		}
		SDLQuiter sdlQuiter(nullptr, [](void*) { SDL_Quit(); });

		if (TTF_Init() != 0)
		{
			SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
			return 1;
		}
		TTFQuiter ttfQuiter(nullptr, [](void*) { TTF_Quit(); });

		SDLWindowPointer window(SDL_CreateWindow(
			"Mario by Pavlo Naichuk",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			windowWidth,
			windowHeight,
			SDL_WINDOW_OPENGL
		), SDL_DestroyWindow);

		if (window == nullptr)
		{
			SDL_Log("Unable to created window: %s", SDL_GetError());
			return 1;
		}
		SDLRendererPointer renderer(SDL_CreateRenderer(window.get(), -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE), SDL_DestroyRenderer);

		if (renderer == nullptr)
		{
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return 1;
		}

		StartMenuState startMenuState(renderer, windowWidth, windowHeight);
		PlayGameState playGameState(renderer);
		EndMenuState endMenuState(renderer, windowWidth, windowHeight);

		GameState* currState = &startMenuState;

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
					currState->ProcessKeyboard(event.key.keysym.sym);
					break;
				}
				}
			}
			currState->Update();
			currState->Render();
		}
	}
}

void MarioGame::EnterState(GameState * newState)
{
}
