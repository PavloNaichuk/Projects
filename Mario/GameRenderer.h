#pragma once

#include "Common.h"

class Mario;
class Enemy;

class GameRenderer
{
public:
	GameRenderer(SDLRendererPointer renderer);

	void BeginRenderFrame();
	void EndRenderFrame();

	void Render(const Mario& mario);
	void Render(const Enemy& enemy);

private:
	SDLRendererPointer mRenderer;

	SDLSurfacePointer mMarioImageSurface;
	SDLTexturePointer mMarioImageTexture;

	SDLSurfacePointer mEnemyImageSurface;
	SDLTexturePointer mEnemyImageTexture;
};
