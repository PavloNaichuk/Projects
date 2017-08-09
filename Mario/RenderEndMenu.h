#pragma once

#include "Common.h"

class RenderEndMenu
{
public:
	RenderEndMenu(SDLRendererPointer& renderer, const int windowWidth, const int windowHeight);
	void render();

private:
	SDLRendererPointer& mRenderer;
	SDL_Color mTextColor;
	TTFFontPointer mGameOverTextFont;
	SDLSurfacePointer mGameOverTextSurface;
	SDLTexturePointer mGameOverTextTexture;
	SDL_Rect mGameOverTextRect;
};
