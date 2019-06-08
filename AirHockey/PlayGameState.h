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
	PlayGameState(SharedRendererPointer renderer, SharedResourceManagerPointer resourceManager);

	void Enter() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Exit() override;

private: 
	/*GameObject CreatePlayerStriker(const Point& center, const Size& size);
	GameObject CreateEnemyStriker(const Point& center, const Size& size);
	GameObject CreatePuck(const Point& center, const Size& size);*/

private:
	SharedRendererPointer mRenderer;
	SharedResourceManagerPointer mResourceManager;

	std::vector<GameObject> mGameObjects;
};
