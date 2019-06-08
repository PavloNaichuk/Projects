#include "pch.h"
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

PlayGameState::PlayGameState(SharedRendererPointer renderer, SharedResourceManagerPointer resourceManager) 
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
{}

void PlayGameState::Enter()
{
	mGameObjects.reserve(NumGameObjects);
	/*mGameObjects.emplace_back(CreatePlayerStriker(Point(0.0f, 0.0f), Size(47.0f, 47.0f)));
	mGameObjects.emplace_back(CreateEnemyStriker(Point(0.0f, 0.0f), Size(46.0f, 47.0f)));
	mGameObjects.emplace_back(CreatePuck(Point(0.0f, 0.0f), Size(29.0f, 30.0f)));*/
}

void PlayGameState::Update(float deltaTime)
{
	for (GameObject& gameObject : mGameObjects)
	{
		MovementComponent* component = gameObject.GetComponent<MovementComponent>(MovementComponent::COMPONENT_ID);
		component->Update(gameObject, deltaTime);
	}
}

void PlayGameState::Render() 
{
	for (GameObject& gameObject : mGameObjects)
	{
		RenderComponent* component = gameObject.GetComponent<RenderComponent>(RenderComponent::COMPONENT_ID);
		component->Render(gameObject);
	}
}

void PlayGameState::Exit()
{

}
/**
GameObject PlayGameState::CreatePlayerStriker(const Point& center, const Size& size) 
{
	GameObject gameObject(PlayerStrikerId);
	gameObject.AddComponent(std::make_unique<PositionComponent>(center));
	gameObject.AddComponent(std::make_unique<SizeComponent>(size));
	gameObject.AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject.AddComponent(std::make_unique<ScoreComponent>(0));
	gameObject.AddComponent(std::make_unique<PlayerStrikerMovement>());
	gameObject.AddComponent(std::make_unique<PlayerStrikerRenderer>(mRenderer, mResourceManager));
	
	return gameObject;
}

GameObject PlayGameState::CreateEnemyStriker(const Point& center, const Size& size) 
{
	GameObject gameObject(EnemyStrikerId);
	gameObject.AddComponent(std::make_unique<PositionComponent>(center));
	gameObject.AddComponent(std::make_unique<SizeComponent>(size));
	gameObject.AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject.AddComponent(std::make_unique<ScoreComponent>(0));
	gameObject.AddComponent(std::make_unique<EnemyStrikerMovement>());
	gameObject.AddComponent(std::make_unique<EnemyStrikerRenderer>(mRenderer, mResourceManager));

	return gameObject;
}

GameObject PlayGameState::CreatePuck(const Point& center, const Size& size) 
{
	GameObject gameObject(PuckId);
	gameObject.AddComponent(std::make_unique<PositionComponent>(center));
	gameObject.AddComponent(std::make_unique<SizeComponent>(size));
	gameObject.AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject.AddComponent(std::make_unique<PuckMovement>());
	gameObject.AddComponent(std::make_unique<PuckRenderer>(mRenderer, mResourceManager));

	return gameObject;
}*/
