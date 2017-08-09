#pragma once

#include "Common.h"

class RenderGamePlay
{
public:
	RenderGamePlay(SDLRendererPointer& renderer);
	void render();

private:
	SDLRendererPointer& mRenderer;
	SDL_Color mTextColor;
	TTFFontPointer mMarTextFont;
	SDLSurfacePointer mMarTextSurface;
	SDLTexturePointer mMarTextTexture;
	SDL_Rect mMarTextRect;
	TTFFontPointer mTimeTextFont;
	SDLSurfacePointer mTimeTextSurface;
	SDLTexturePointer mTimeTextTexture;
	SDL_Rect mTimeTextRect;
};

