#include "pch.h"
#include "SDL.h"
#include <stdio.h>
#include "Common.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

SDL_Texture* LoadImage(std::string file) 
{
	SDL_Surface *loadedImage = nullptr;
	SDL_Texture *texture = nullptr;
	loadedImage = SDL_LoadBMP(file.c_str());
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	else
		std::cout << SDL_GetError() << std::endl;
	return texture;
}

void ApplySurface(int x, int y, SDL_Texture* tex, SDL_Renderer* rend) 
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(rend, tex, NULL, &pos);
}

int main(int argc, char* argv[]) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}
	window = SDL_CreateWindow("AirHockey", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 2;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 3;
	}

	SDL_Texture* player = LoadImage("Resources/player.bmp");
	SDL_Texture* enemy = LoadImage("Resources/enemy.jpg");
	SDL_Texture* puck = LoadImage("Resources/puck.jpg");

	SDL_RenderClear(renderer);

	int bW, bH;
	SDL_QueryTexture(player, NULL, NULL, &bW, &bH);
	ApplySurface(0, 0, player, renderer);
	
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
	
	SDL_DestroyTexture(player);
	SDL_DestroyTexture(enemy);
	SDL_DestroyTexture(puck);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}