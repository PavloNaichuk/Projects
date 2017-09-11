#include "GameRenderer.h"
#include "Mario.h"
#include "Enemy.h"
#include "FireBall.h"

GameRenderer::GameRenderer(SDLRendererPointer renderer) 
	: mRenderer(renderer)
	, mMarioImageSurface(IMG_Load("Resources/Images/mario.JPG"), SDL_FreeSurface)
	, mMarioImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioImageSurface.get()), SDL_DestroyTexture)
	, mEnemyImageSurface(IMG_Load("Resources/Images/owl.JPG"), SDL_FreeSurface)
	, mEnemyImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mEnemyImageSurface.get()), SDL_DestroyTexture)
	, mFireBallImageSurface(IMG_Load("Resources/Images/fireBall.JPG"), SDL_FreeSurface)
	, mFireBallImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mFireBallImageSurface.get()), SDL_DestroyTexture)
{
	if (mMarioImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mMarioImageTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	if (mEnemyImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mEnemyImageTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	if(mFireBallImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mFireBallImageTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}
}

void GameRenderer::BeginRenderFrame()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 255, 0);
	SDL_RenderClear(mRenderer.get());
}

void GameRenderer::EndRenderFrame()
{
	SDL_RenderPresent(mRenderer.get());
}

void GameRenderer::Render(const Mario& mario)
{
	Size size = 2.0f * mario.mHalfSize;
	Point topLeft = mario.mCenter - mario.mHalfSize;
	SDL_Rect destRect = {(int)topLeft.mX, (int)topLeft.mY, (int)size.mX, (int)size.mY};

	SDL_RenderCopy(mRenderer.get(), mMarioImageTexture.get(), nullptr, &destRect);
}

void GameRenderer::Render(const Enemy& enemy)
{
	Size size = 2.0f * enemy.mHalfSize;
	Point topLeft = enemy.mCenter - enemy.mHalfSize;
	SDL_Rect destRect = {(int)topLeft.mX, (int)topLeft.mY, (int)size.mX, (int)size.mY};

	SDL_RenderCopy(mRenderer.get(), mEnemyImageTexture.get(), nullptr, &destRect);
}

void GameRenderer::Render(const FireBall& fireBall)
{
	Size size = 2.0f * fireBall.mHalfSize;
	Point topLeft = fireBall.mCenter - fireBall.mHalfSize;
	SDL_Rect destRect = { (int)topLeft.mX, (int)topLeft.mY, (int)size.mX, (int)size.mY };

	SDL_RenderCopy(mRenderer.get(), mFireBallImageTexture.get(), nullptr, &destRect);
}
