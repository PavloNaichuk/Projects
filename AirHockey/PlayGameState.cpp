#include "pch.h"
#include "Config.h"
#include "PlayGameState.h"
#include "RenderComponent.h"
#include "AIComponent.h"
#include "PositionComponent.h"
#include "SizeComponent.h"
#include "PlayerStrikerRenderer.h"
#include "EnemyStrikerRenderer.h"
#include "PuckRenderer.h"
#include "GoalRenderer.h"
#include "PlayerStrikerMovement.h"
#include "EnemyStrikerMovement.h"
#include "PuckMovement.h"
#include "GoalLogic.h"
#include "StrikerPhysics.h"
#include "PuckPhysics.h"
#include "GoalPhysics.h"
#include "ScoreComponent.h"
#include "VelocityComponent.h"
#include "GameObject.h"

enum GameObjectId 
{
	PLAYER_STRIKER_ID,
	PLAYER_GOAL_ID,
	ENEMY_STRIKER_ID,
	ENEMY_GOAL_ID,
	PUCK_ID,
	NUM_GAME_OBJECTS
};

PlayGameState::PlayGameState(SharedRenderer renderer, SharedResourceManager resourceManager) 
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
{}

void PlayGameState::Enter()
{
	mBoardTexture = mResourceManager->GetTexture(ResourceManager::BOARD_ID);
	assert(mBoardTexture);

	mGameObjects.reserve(NUM_GAME_OBJECTS);
	mGameObjects.emplace_back(CreatePlayerStriker(Point(BOARD_WIDTH - STRIKER_RADIUS, BOARD_HEIGHT / 2), STRIKER_RADIUS));
	mGameObjects.emplace_back(CreatePlayerGoal(Point(BOARD_WIDTH, BOARD_HEIGHT / 2), GOAL_RADIUS));
	mGameObjects.emplace_back(CreateEnemyStriker(Point(STRIKER_RADIUS, BOARD_HEIGHT / 2), STRIKER_RADIUS));
	mGameObjects.emplace_back(CreateEnemyGoal(Point(0.0f, BOARD_HEIGHT / 2), GOAL_RADIUS));
	mGameObjects.emplace_back(CreatePuck(Point(BOARD_WIDTH - BOARD_WIDTH / 4, BOARD_HEIGHT / 2), PUCK_RADIUS));
}

void PlayGameState::Update(float deltaTime)
{
	for (auto& gameObject : mGameObjects)
	{
		AIComponent* component = gameObject->GetComponent<AIComponent>(AIComponent::COMPONENT_ID);
		component->Update(*gameObject, deltaTime);
	}

	for (auto& gameObject : mGameObjects) 
	{
		PhysicsComponent* component = gameObject->GetComponent<PhysicsComponent>(PhysicsComponent::COMPONENT_ID);
		component->Update(*gameObject);
	}
}

void PlayGameState::Render() 
{
	SDL_Rect destRect = {0, 0, BOARD_WIDTH, BOARD_HEIGHT};

	SDL_SetTextureBlendMode(mBoardTexture.get(), SDL_BLENDMODE_NONE);
	SDL_RenderCopy(mRenderer.get(), mBoardTexture.get(), nullptr, &destRect);

	for (auto& gameObject : mGameObjects)
	{
		RenderComponent* component = gameObject->GetComponent<RenderComponent>(RenderComponent::COMPONENT_ID);
		component->Render(*gameObject);
	}

	SDL_RenderPresent(mRenderer.get());
}

void PlayGameState::Exit()
{

}

UniqueGameObject PlayGameState::CreatePlayerStriker(const Point& center, float radius)
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(PLAYER_STRIKER_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(radius));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<ScoreComponent>(0));
	gameObject->AddComponent(std::make_unique<PlayerStrikerMovement>());
	gameObject->AddComponent(std::make_unique<PlayerStrikerRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<StrikerPhysics>(Region(Point(BOARD_WIDTH / 2, 0), Point(BOARD_WIDTH, BOARD_HEIGHT))));
	
	return gameObject;
}

UniqueGameObject PlayGameState::CreatePlayerGoal(const Point& center, float radius) 
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(PLAYER_GOAL_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(radius));
	gameObject->AddComponent(std::make_unique<GoalLogic>());
	gameObject->AddComponent(std::make_unique<GoalPhysics>());
	gameObject->AddComponent(std::make_unique<GoalRenderer>());

	return gameObject;
}

UniqueGameObject PlayGameState::CreateEnemyStriker(const Point& center, float radius)
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(ENEMY_STRIKER_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(radius));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<ScoreComponent>(0));
	gameObject->AddComponent(std::make_unique<EnemyStrikerMovement>());
	gameObject->AddComponent(std::make_unique<EnemyStrikerRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<StrikerPhysics>(Region(Point(0, 0), Point(BOARD_WIDTH / 2, BOARD_HEIGHT))));

	return gameObject;
}

UniqueGameObject PlayGameState::CreateEnemyGoal(const Point& center, float radius) 
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(ENEMY_GOAL_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(radius));
	gameObject->AddComponent(std::make_unique<GoalLogic>());
	gameObject->AddComponent(std::make_unique<GoalPhysics>());
	gameObject->AddComponent(std::make_unique<GoalRenderer>());

	return gameObject;
}

UniqueGameObject PlayGameState::CreatePuck(const Point& center, float radius)
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(PUCK_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(radius));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<PuckMovement>());
	gameObject->AddComponent(std::make_unique<PuckRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<PuckPhysics>());

	return gameObject;
}
