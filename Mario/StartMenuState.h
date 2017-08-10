#pragma once

#include "Common.h"
#include "GameState.h"
#include "StartMenuStateListener.h"

class StartMenuState : public GameState
{
public:
	StartMenuState(StartMenuStateListener& listener, SDLRendererPointer& renderer, const int windowWidth, const int windowHeight);

    void Enter() override;
	void Exit() override;
	void ProcessKeyboard(SDL_Keycode key) override;
	void Update() override;
	void Render() override;

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

	StartMenuStateListener& mListener;
};
