#pragma once

#include "Common.h"
#include "IGameState.h"

class RenderEndMenu : public IGameState
{
public:
	RenderEndMenu(SDLRendererPointer& renderer, const int windowWidth, const int windowHeight);
	virtual void enter();
	virtual void exit();
	virtual void processKeyboard(SDL_Keycode key);
	virtual void update();
	virtual void render();

private:
	SDLRendererPointer& mRenderer;
	SDL_Color mTextColor;
	TTFFontPointer mGameOverTextFont;
	SDLSurfacePointer mGameOverTextSurface;
	SDLTexturePointer mGameOverTextTexture;
	SDL_Rect mGameOverTextRect;
};
