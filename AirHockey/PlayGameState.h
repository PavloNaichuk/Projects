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
	GameObjectUniquePointer CreatePlayerStriker(const Point& center, const Size& size);
	GameObjectUniquePointer CreateEnemyStriker(const Point& center, const Size& size);
	GameObjectUniquePointer CreatePuck(const Point& center, const Size& size);

private:
	SharedRenderer mRenderer;
	SharedResourceManager mResourceManager;
	SharedTexture mBoardTexture;

	std::vector<GameObjectUniquePointer> mGameObjects;
};
