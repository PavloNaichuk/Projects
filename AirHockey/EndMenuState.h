#pragma once

#include "GameState.h"
#include "Common.h"

class EndMenuState : public GameState
{
public:
	EndMenuState(SharedRenderer renderer, const int windowWidth, const int windowHeight);

	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;

private:
	SharedRenderer mRenderer;
	SDL_Color mTextColor;
	SharedFont mGameOverTextFont;
	SharedSurface mGameOverTextSurface;
	SharedTexture mGameOverTextTexture;
	SDL_Rect mGameOverTextRect;
};

