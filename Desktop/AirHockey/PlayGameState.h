#pragma once

#include "GameState.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Common.h"
#include "Math.h"
#include <vector>

class PlayGameState : public GameState
{
public:
	PlayGameState(SharedRenderer renderer, SharedResourceManager resourceManager);

	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;

private: 
	UniqueGameObject CreatePlayerStriker(const Point& center, float radius);
	UniqueGameObject CreatePlayerGoal(const Point& center, float radius);
	UniqueGameObject CreateEnemyStriker(const Point& center, float radius);
	UniqueGameObject CreateEnemyGoal(const Point& center, float radius);
	UniqueGameObject CreatePuck(const Point& center, float radius);

	UniqueGameObject CreateScoreUI(const Point& topLeft, int width, int height);
	UniqueGameObject CreateTimerUI(const Point& topLeft, int width, int height);

private:
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;
	SharedTexture mBoardTexture;
	
	GameObjectList mUIObjects;
	GameObjectList mGameObjects;
};
