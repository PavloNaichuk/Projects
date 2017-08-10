#pragma once

#include "Common.h"
#include "IGameState.h"

class RenderStartMenu : public IGameState
{
public:
	RenderStartMenu(SDLRendererPointer& renderer, const int windowWidth, const int windowHeight);
	virtual void enter();
	virtual void exit();
	virtual void processKeyboard(SDL_Keycode key);
	virtual void update();
	virtual void render();

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
