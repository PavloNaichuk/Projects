#pragma once

#include "Common.h"
#include "GameState.h"

class EndMenuStateListener
{
public:
	virtual ~EndMenuStateListener() {}
	virtual void OnGameOver() = 0;
};

class EndMenuState : public GameState
{
public:
	EndMenuState(EndMenuStateListener& listener, SDLRendererPointer renderer, const int windowWidth, const int windowHeight);

	void Enter() override;
	void Exit() override;
	void ProcessKeyboard(SDL_Keycode key) override;
	void Update() override;
	void Render() override;

private:
	EndMenuStateListener& mListener;
	SDLRendererPointer mRenderer;

	SDL_Color mTextColor;

	TTFFontPointer mGameOverTextFont;
	SDLSurfacePointer mGameOverTextSurface;
	SDLTexturePointer mGameOverTextTexture;
	SDL_Rect mGameOverTextRect;
};
