#pragma once

#include "GameState.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Common.h"
#include "Vector.h"
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

private:
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;
	SharedTexture mBoardTexture;

	std::vector<UniqueGameObject> mGameObjects;
};
