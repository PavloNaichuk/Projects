#include "pch.h"
#include "Config.h"
#include "PlayGameState.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "SizeComponent.h"
#include "PlayerStrikerRenderer.h"
#include "EnemyStrikerRenderer.h"
#include "PuckRenderer.h"
#include "PlayerStrikerMovement.h"
#include "EnemyStrikerMovement.h"
#include "PuckMovement.h"
#include "StrikerPhysics.h"
#include "PuckPhysics.h"
#include "ScoreComponent.h"
#include "VelocityComponent.h"
#include "GameObject.h"

enum GameObjectId 
{
	PlayerStrikerId,
	EnemyStrikerId,
	PuckId,
	NumGameObjects
};

PlayGameState::PlayGameState(SharedRenderer renderer, SharedResourceManager resourceManager) 
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
{}

void PlayGameState::Enter()
{
	mBoardTexture = mResourceManager->GetTexture(ResourceManager::BoardId);
	assert(mBoardTexture);

	mGameObjects.reserve(NumGameObjects);
	mGameObjects.emplace_back(CreatePlayerStriker(Point(0.0f, 0.0f), Size(47.0f, 47.0f)));
	mGameObjects.emplace_back(CreateEnemyStriker(Point(0.0f, 0.0f), Size(46.0f, 47.0f)));
	mGameObjects.emplace_back(CreatePuck(Point(0.0f, 0.0f), Size(29.0f, 30.0f)));
}

void PlayGameState::Update(float deltaTime)
{
	for (auto& gameObject : mGameObjects)
	{
		MovementComponent* component = gameObject->GetComponent<MovementComponent>(MovementComponent::COMPONENT_ID);
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

GameObjectUniquePointer PlayGameState::CreatePlayerStriker(const Point& center, const Size& size)
{
	GameObjectUniquePointer gameObject = std::make_unique<GameObject>(PlayerStrikerId);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(size));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<ScoreComponent>(0));
	gameObject->AddComponent(std::make_unique<PlayerStrikerMovement>());
	gameObject->AddComponent(std::make_unique<PlayerStrikerRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<StrikerPhysics>());
	
	return gameObject;
}

GameObjectUniquePointer PlayGameState::CreateEnemyStriker(const Point& center, const Size& size)
{
	GameObjectUniquePointer gameObject = std::make_unique<GameObject>(EnemyStrikerId);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(size));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<ScoreComponent>(0));
	gameObject->AddComponent(std::make_unique<EnemyStrikerMovement>());
	gameObject->AddComponent(std::make_unique<EnemyStrikerRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<StrikerPhysics>());

	return gameObject;
}

GameObjectUniquePointer PlayGameState::CreatePuck(const Point& center, const Size& size)
{
	GameObjectUniquePointer gameObject = std::make_unique<GameObject>(PuckId);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<SizeComponent>(size));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<PuckMovement>());
	gameObject->AddComponent(std::make_unique<PuckRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<PuckPhysics>());

	return gameObject;
}
