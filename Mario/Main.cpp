#include <SDL.h>
#include <SDL_ttf.h>

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0) 
	{
		SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"Mario by Pavlo Naichuk",                  
		SDL_WINDOWPOS_UNDEFINED,           
		SDL_WINDOWPOS_UNDEFINED,          
		640,                               
		480,                               
		SDL_WINDOW_OPENGL                  
	);

	if (window == nullptr) 
	{
		SDL_Log("Unable to created window: %s", SDL_GetError());
		return 1;
	}
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

	if (renderer == nullptr)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("Resources/Fonts/Arial.TTF", 16);
	if (font == nullptr) 
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
		return 1;
	}

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
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
	
	TTF_CloseFont(font);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}