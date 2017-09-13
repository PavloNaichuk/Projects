#include "GameRenderer.h"
#include "Mario.h"
#include "Enemy.h"
#include "FireBall.h"
#include "GoldCoin.h"
#include "Config.h"

GameRenderer::GameRenderer(SDLRendererPointer renderer) 
	: mRenderer(renderer)
	, mMarioLeftImageSurface(IMG_Load("Resources/Images/marioLeft.JPG"), SDL_FreeSurface)
	, mMarioLeftImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioLeftImageSurface.get()), SDL_DestroyTexture)
	, mMarioRightImageSurface(IMG_Load("Resources/Images/marioRight.JPG"), SDL_FreeSurface)
	, mMarioRightImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mMarioRightImageSurface.get()), SDL_DestroyTexture)
	, mEnemyImageSurface(IMG_Load("Resources/Images/owl.JPG"), SDL_FreeSurface)
	, mEnemyImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mEnemyImageSurface.get()), SDL_DestroyTexture)
	, mFireBallImageSurface(IMG_Load("Resources/Images/fireBall.JPG"), SDL_FreeSurface)
	, mFireBallImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mFireBallImageSurface.get()), SDL_DestroyTexture)
	, mGoldCoinImageSurface(IMG_Load("Resources/Images/fireBall.JPG"), SDL_FreeSurface)
	, mGoldCoinImageTexture(SDL_CreateTextureFromSurface(renderer.get(), mGoldCoinImageSurface.get()), SDL_DestroyTexture)
{
	if (mMarioLeftImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mMarioLeftImageTexture == nullptr)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
	}

	if (mMarioRightImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mMarioRightImageTexture == nullptr)
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

	if (mGoldCoinImageSurface == nullptr)
	{
		SDL_Log("IMG_Load error: %s", IMG_GetError());
	}
	if (mGoldCoinImageTexture == nullptr)
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
	if (mario.mDirection > 0.0f)
		SDL_RenderCopy(mRenderer.get(), mMarioRightImageTexture.get(), nullptr, &destRect);
	else
		SDL_RenderCopy(mRenderer.get(), mMarioLeftImageTexture.get(), nullptr, &destRect);	
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
