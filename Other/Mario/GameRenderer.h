#pragma once

#include "Common.h"

class Mario;
class Enemy;
class FireBall;
class GoldCoin;
class PowerUpToLevel2;
class PowerUpToLevel3;

class GameRenderer
{
public:
	GameRenderer(SDLRendererPointer renderer);

	void BeginRenderFrame();
	void EndRenderFrame();

	void Render(const Mario& mario);
	void Render(const Enemy& enemy);
	void Render(const FireBall& fireBall);
	void Render(const GoldCoin& goldCoin);
	void Render(const PowerUpToLevel2& powerUpToLevel2);
	void Render(const PowerUpToLevel3& powerUpToLevel3);

private:
	SDLRendererPointer mRenderer;

	SDLSurfacePointer mMarioLeftImageSurface;
	SDLTexturePointer mMarioLeftImageTexture;
	SDLSurfacePointer mMarioRightImageSurface;
	SDLTexturePointer mMarioRightImageTexture;

	SDLSurfacePointer mMarioLeftImageLevel3Surface;
	SDLTexturePointer mMarioLeftImageLevel3Texture;
	SDLSurfacePointer mMarioRightImageLevel3Surface;
	SDLTexturePointer mMarioRightImageLevel3Texture;

	SDLSurfacePointer mEnemyImageSurface;
	SDLTexturePointer mEnemyImageTexture;

	SDLSurfacePointer mFireBallImageSurface;
	SDLTexturePointer mFireBallImageTexture;

	SDLSurfacePointer mGoldCoinImageSurface;
	SDLTexturePointer mGoldCoinImageTexture;

	SDLSurfacePointer mPowerUpToLevel2ImageSurface;
	SDLTexturePointer mPowerUpToLevel2ImageTexture;

	SDLSurfacePointer mPowerUpToLevel3ImageSurface;
	SDLTexturePointer mPowerUpToLevel3ImageTexture;
};
