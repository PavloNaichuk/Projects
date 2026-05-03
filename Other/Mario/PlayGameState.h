#pragma once

#include "Common.h"
#include "Config.h"
#include "GameState.h"
#include "GameUnit.h"
#include "GameWorld.h"
#include "GameRenderer.h"

class PlayGameStateListener
{
public:
	virtual ~PlayGameStateListener() {}

	virtual void OnLevelLose() = 0;
	virtual void OnLevelComplete() = 0;
};

class PlayGameState : public GameState
{
public:
	PlayGameState(PlayGameStateListener& listener, SDLRendererPointer renderer);
	~PlayGameState() override;

	void Enter() override;
	void Exit() override;
	void ProcessKeyPressed(SDL_Keycode key) override;
	void ProcessKeyReleased(SDL_Keycode key) override;
	void Update(float elapsedTime) override;
	void Render() override;

private:
	PlayGameStateListener& mListener;

	SDL_Color mTextColor;

	TTFFontPointer mMarioTextFont;
	SDLSurfacePointer mMarioTextSurface;
	SDLTexturePointer mMarioTextTexture;
	SDL_Rect mMarioTextRect;

	TTFFontPointer mTimeTextFont;
	SDLSurfacePointer mTimeTextSurface;
	SDLTexturePointer mTimeTextTexture;
	SDL_Rect mTimeTextRect;

	SDLSurfacePointer mEnemyImageSurface;
	SDLTexturePointer mEnemyImageTexture;

	GameRenderer mGameRenderer;
	GameWorld mGameWorld;
};