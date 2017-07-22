#include <SDL.h>
#include <SDL_ttf.h>
#include <conio.h>
#include <iostream>

void ClearMario(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* marioTextFont, TTF_Font* enterTextFont, 
	SDL_Surface* marioTextSurface, SDL_Surface* enterTextSurface)
{
	TTF_CloseFont(marioTextFont);
	TTF_CloseFont(enterTextFont);
	TTF_Quit();

	SDL_FreeSurface(marioTextSurface);
	SDL_FreeSurface(enterTextSurface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char** argv)
{
	const int windowWidth = 1280;
	const int windowHeight = 720;

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
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);


	if (window == nullptr)
	{
		SDL_Log("Unable to created window: %s", SDL_GetError());
		ClearMario(window, nullptr, nullptr, nullptr, nullptr, nullptr);
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

	if (renderer == nullptr)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		ClearMario(window, renderer, nullptr, nullptr, nullptr, nullptr);
		return 1;
	}

	TTF_Font* marioTextFont = TTF_OpenFont("Resources/Fonts/Arial.TTF", 130);
	if (marioTextFont == nullptr)
	{
		SDL_Log("Unable to create font: %s", TTF_GetError());
		ClearMario(window, renderer, marioTextFont, nullptr, nullptr, nullptr);
		return 1;
	}

	SDL_Color textColor = { 255, 255, 255, 255 };
	SDL_Surface* marioTextSurface = TTF_RenderText_Solid(marioTextFont, "Mario", textColor);
	if (marioTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
		ClearMario(window, renderer, marioTextFont, nullptr, marioTextSurface, nullptr);
		return 1;
	}

	SDL_Texture* marioTextTexture = SDL_CreateTextureFromSurface(renderer, marioTextSurface);
	if (marioTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", TTF_GetError());
		ClearMario(window, renderer, marioTextFont, nullptr, marioTextSurface, nullptr);
		return 1;
	}
	SDL_Rect marioTextRect;
	SDL_QueryTexture(marioTextTexture, nullptr, nullptr, &marioTextRect.w, &marioTextRect.h);
	marioTextRect.x = windowWidth / 2 - marioTextRect.w / 2;
	marioTextRect.y = windowHeight / 2 - marioTextRect.h / 2 - 50;


	TTF_Font* enterTextFont = TTF_OpenFont("Resources/Fonts/Arial.TTF", 30);
	SDL_Surface* enterTextSurface = TTF_RenderText_Solid(enterTextFont, "Press Space to enter", textColor);
	if (enterTextSurface == nullptr)
	{
		SDL_Log("Unable to create surface: %s", TTF_GetError());
		ClearMario(window, renderer, marioTextFont, enterTextFont, marioTextSurface, enterTextSurface);
		return 1;
	}

	SDL_Texture* enterTextTexture = SDL_CreateTextureFromSurface(renderer, enterTextSurface);
	if (enterTextTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", TTF_GetError());
		ClearMario(window, renderer, marioTextFont, enterTextFont, marioTextSurface, enterTextSurface);
		return 1;
	}
	SDL_Rect enterTextRect;
	SDL_QueryTexture(enterTextTexture, nullptr, nullptr, &enterTextRect.w, &enterTextRect.h);
	enterTextRect.x = windowWidth / 2 - enterTextRect.w / 2;
	enterTextRect.y = windowHeight / 2 - enterTextRect.h / 2 + marioTextRect.h;

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
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
				{
					std::cout << "Space";
					break;
				}
				}
				break;
			}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, marioTextTexture, nullptr, &marioTextRect);
		SDL_RenderCopy(renderer, enterTextTexture, nullptr, &enterTextRect);
		SDL_RenderPresent(renderer);
	}
	return 0;
}

