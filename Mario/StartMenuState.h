#pragma once

#include "Common.h"
#include "GameState.h"

class StartMenuStateListener
{
public:
	virtual ~StartMenuStateListener() {}
	virtual void OnGameStart() = 0;
};

class StartMenuState : public GameState
{
public:
	StartMenuState(StartMenuStateListener& listener, SDLRendererPointer renderer, const int windowWidth, const int windowHeight);

    void Enter() override;
	void Exit() override;
	void ProcessKeyboard(SDL_Keycode key) override;
	void Update() override;
	void Render() override;

private:
	StartMenuStateListener& mListener;
	SDLRendererPointer mRenderer;

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
