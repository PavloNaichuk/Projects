#pragma once

#include "Common.h"

class Mario;
class Enemy;
class FireBall;

class GameRenderer
{
public:
	GameRenderer(SDLRendererPointer renderer);

	void BeginRenderFrame();
	void EndRenderFrame();

	void Render(const Mario& mario);
	void Render(const Enemy& enemy);
	void Render(const FireBall& fireBall);

private:
	SDLRendererPointer mRenderer;

	SDLSurfacePointer mMarioLeftImageSurface;
	SDLTexturePointer mMarioLeftImageTexture;
	SDLSurfacePointer mMarioRightImageSurface;
	SDLTexturePointer mMarioRightImageTexture;

	SDLSurfacePointer mEnemyImageSurface;
	SDLTexturePointer mEnemyImageTexture;

	SDLSurfacePointer mFireBallImageSurface;
	SDLTexturePointer mFireBallImageTexture;

	SDLSurfacePointer mGoldCoinImageSurface;
	SDLTexturePointer mGoldCoinImageTexture;
};
