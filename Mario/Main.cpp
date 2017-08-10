#include "RenderStartMenu.h"
#include "RenderGamePlay.h"
#include "RenderEndMenu.h"
#include "Common.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <conio.h>
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <sstream>

int main(int argc, char** argv)
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
	
	RenderStartMenu renderStartMenu(renderer, windowWidth, windowHeight);
	
	RenderGamePlay renderGamePlay(renderer);

	RenderEndMenu renderEndMenu(renderer, windowWidth, windowHeight);

	SDL_Event event;

	IGameState* currState;
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
					switch (event.key.keysym.sym)
					{
						currState->processKeyboard(SDLK_SPACE);
						currState->update();
						currState->render();
					}
				}
			}
		}
	}
	return 0;
}

