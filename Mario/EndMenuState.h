#pragma once

#include "Common.h"
#include "GameState.h"

class EndMenuState : public GameState
{
public:
	EndMenuState(SDLRendererPointer& renderer, const int windowWidth, const int windowHeight);

	void Enter() override;
	void Exit() override;
	void ProcessKeyboard(SDL_Keycode key) override;
	void Update() override;
	void Render() override;

private:
	SDLRendererPointer& mRenderer;
	SDL_Color mTextColor;
	TTFFontPointer mGameOverTextFont;
	SDLSurfacePointer mGameOverTextSurface;
	SDLTexturePointer mGameOverTextTexture;
	SDL_Rect mGameOverTextRect;
};
