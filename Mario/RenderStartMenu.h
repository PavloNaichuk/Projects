#pragma once

#include "Common.h"

class RenderStartMenu 
{
public:
	RenderStartMenu(SDLRendererPointer& renderer, const int windowWidth, const int windowHeight);
	void render();

private:
	SDLRendererPointer& mRenderer;
	SDL_Color mTextColor;
	TTFFontPointer mMarioTextFont;
	SDLSurfacePointer mMarioTextSurface;
	SDLTexturePointer mMarioTextTexture;
	SDL_Rect mMarioTextRect;
	
	TTFFontPointer mEnterTextFont;
	SDLSurfacePointer mEnterTextSurface;
	SDLTexturePointer mEnterTextTexture;
	SDL_Rect mEnterTextRect;
};
