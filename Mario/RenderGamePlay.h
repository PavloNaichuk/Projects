#pragma once

#include "Common.h"
#include "IGameState.h"

class RenderGamePlay : public IGameState
{
public:
	RenderGamePlay(SDLRendererPointer& renderer);
	virtual void enter();
	virtual void exit();
	virtual void processKeyboard(SDL_Keycode key);
	virtual void update();
	virtual void render();

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

