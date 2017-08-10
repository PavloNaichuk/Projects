#pragma once

#include "Common.h"
#include "GameState.h"
#include "PlayGameStateListener.h"

class PlayGameState : public GameState
{
public:
	PlayGameState(PlayGameStateListener& listener, SDLRendererPointer& renderer);

	void Enter() override;
	void Exit() override;
	void ProcessKeyboard(SDL_Keycode key) override;
	void Update() override;
	void Render() override;

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

	PlayGameStateListener& mListener;
};

