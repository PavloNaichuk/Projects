#pragma once

#include "GameState.h"
#include "Common.h"
#include "ResourceManager.h"
#include "GameObject.h"

class EndMenuState : public GameState
{
public:
	EndMenuState(SharedRenderer renderer, SharedResourceManager resourceManager);

	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;

private:
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;
	SharedFont mGameOverTextFont;
	SharedSurface mGameOverTextSurface;
	SharedTexture mGameOverTextTexture;
	SDL_Rect mGameOverTextRect;
};

