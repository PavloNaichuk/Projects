#include "pch.h"
#include "Config.h"
#include "PlayGameState.h"
#include "RenderComponent.h"
#include "AIComponent.h"
#include "PositionComponent.h"
#include "RadiusComponent.h"
#include "BoxComponent.h"
#include "PlayerStrikerRenderer.h"
#include "EnemyStrikerRenderer.h"
#include "PuckRenderer.h"
#include "GoalRenderer.h"
#include "ScoreUIRenderer.h"
#include "ScoreUIData.h"
#include "TimerUIData.h"
#include "TimerUIRenderer.h"
#include "PlayerStrikerMovement.h"
#include "EnemyStrikerMovement.h"
#include "PuckMovement.h"
#include "GoalLogic.h"
#include "ScoreUILogic.h"
#include "TimerUILogic.h"
#include "StrikerPhysics.h"
#include "PuckPhysics.h"
#include "GoalPhysics.h"
#include "ScoreComponent.h"
#include "VelocityComponent.h"
#include "GameObject.h"
#include "EventCenter.h"

PlayGameState::PlayGameState(SharedRenderer renderer, SharedResourceManager resourceManager) 
	: mRenderer(renderer)
	, mResourceManager(resourceManager)
{}

void PlayGameState::Enter()
{
	mBoardTexture = mResourceManager->GetTexture(ResourceManager::BOARD_ID);
	assert(mBoardTexture);

	mGameObjects.resize(NUM_GAME_OBJECTS);
	mGameObjects[PLAYER_STRIKER_ID] = CreatePlayerStriker(Point(BOARD_WIDTH - STRIKER_RADIUS, BOARD_HEIGHT / 2), STRIKER_RADIUS);
	mGameObjects[PLAYER_GOAL_ID] = CreatePlayerGoal(Point(BOARD_WIDTH, BOARD_HEIGHT / 2), GOAL_RADIUS);
	mGameObjects[ENEMY_STRIKER_ID] = CreateEnemyStriker(Point(STRIKER_RADIUS, BOARD_HEIGHT / 2), STRIKER_RADIUS);
	mGameObjects[ENEMY_GOAL_ID] = CreateEnemyGoal(Point(0.0f, BOARD_HEIGHT / 2), GOAL_RADIUS);
	mGameObjects[PUCK_ID] = CreatePuck(Point(BOARD_WIDTH - BOARD_WIDTH / 4, BOARD_HEIGHT / 2), PUCK_RADIUS);

	mUIObjects.resize(NUM_UI_OBJECTS);
	mUIObjects[SCORE_UI_ID] = CreateScoreUI(Point(0.0f, 0.0f), BOARD_WIDTH / 2, UI_AREA_HEIGHT);
	mUIObjects[TIMER_UI_ID] = CreateTimerUI(Point(BOARD_WIDTH / 2, 0.0f), BOARD_WIDTH / 2, UI_AREA_HEIGHT);

	auto handleEvent = [this](const Event& event)
	{
		if (event.mEventId == Event::GOAL_SCORED_ID) 
		{
			ScoreUIData* UIData = mUIObjects[SCORE_UI_ID]->GetComponent<ScoreUIData>(ScoreUIData::COMPONENT_ID);

			if (event.mSenderId == ENEMY_GOAL_ID)
			{
				 ScoreComponent* scoreComponent = mGameObjects[PLAYER_STRIKER_ID]->GetComponent<ScoreComponent>(ScoreComponent::COMPONENT_ID);
				 scoreComponent->Set(scoreComponent->Get() + 1);

				 UIData->SetPlayerScore(scoreComponent->Get());
			}
			else 
			{
				ScoreComponent* scoreComponent = mGameObjects[ENEMY_STRIKER_ID]->GetComponent<ScoreComponent>(ScoreComponent::COMPONENT_ID);
				scoreComponent->Set(scoreComponent->Get() + 1);
				
				UIData->SetEnemyScore(scoreComponent->Get());
			}
		}
	};
	EventCenter::GetInstance().Subscribe(handleEvent);
}

void PlayGameState::Update(float deltaTime)
{
	for (auto& gameObject : mGameObjects)
	{
		AIComponent* component = gameObject->GetComponent<AIComponent>(AIComponent::COMPONENT_ID);
		component->Update(*gameObject, deltaTime, mGameObjects);
	}

	for (auto& gameObject : mGameObjects)
	{
		PhysicsComponent* component = gameObject->GetComponent<PhysicsComponent>(PhysicsComponent::COMPONENT_ID);
		component->Update(*gameObject, mGameObjects);
	}

	for (auto& gameObject : mUIObjects) 
	{
		AIComponent* component = gameObject->GetComponent<AIComponent>(AIComponent::COMPONENT_ID);
		component->Update(*gameObject, deltaTime, mUIObjects);
	}
}

void PlayGameState::Render() 
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 0);
	SDL_RenderClear(mRenderer.get());

	SDL_Rect UIViewport = {0, 0, BOARD_WIDTH, UI_AREA_HEIGHT};
	SDL_RenderSetViewport(mRenderer.get(), &UIViewport);

	for (auto& gameObject : mUIObjects) 
	{
		RenderComponent* component = gameObject->GetComponent<RenderComponent>(RenderComponent::COMPONENT_ID);
		component->Render(*gameObject);
	}

	SDL_Rect boardViewport = {0, UI_AREA_HEIGHT, BOARD_WIDTH, BOARD_HEIGHT};
	SDL_RenderSetViewport(mRenderer.get(), &boardViewport);

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
	gameObject->AddComponent(std::make_unique<RadiusComponent>(radius));
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
	gameObject->AddComponent(std::make_unique<RadiusComponent>(radius));
	gameObject->AddComponent(std::make_unique<GoalLogic>());
	gameObject->AddComponent(std::make_unique<GoalPhysics>());
	gameObject->AddComponent(std::make_unique<GoalRenderer>());

	return gameObject;
}

UniqueGameObject PlayGameState::CreateEnemyStriker(const Point& center, float radius)
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(ENEMY_STRIKER_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<RadiusComponent>(radius));
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
	gameObject->AddComponent(std::make_unique<RadiusComponent>(radius));
	gameObject->AddComponent(std::make_unique<GoalLogic>());
	gameObject->AddComponent(std::make_unique<GoalPhysics>());
	gameObject->AddComponent(std::make_unique<GoalRenderer>());

	return gameObject;
}

UniqueGameObject PlayGameState::CreatePuck(const Point& center, float radius)
{
	std::vector<BoardWall> boardWalls = {
		BoardWall(Point(0.0f, 0.0f), Point(BOARD_WIDTH, 0.0f), Vector(0.0f, 1.0f)), // Top
		BoardWall(Point(0.0f, BOARD_HEIGHT), Point(BOARD_WIDTH, BOARD_HEIGHT), Vector(0.0f, -1.0f)), // Bottom 
		BoardWall(Point(0.0f, 0.0f), Point(0.0f, GOAL_TOP_OFFSET), Vector(1.0f, 0.0f)), // Top left
		BoardWall(Point(0.0f, GOAL_BOTTOM_OFFSET), Point(0.0f, BOARD_HEIGHT), Vector(1.0f, 0.0f)), // Bottom left
		BoardWall(Point(BOARD_WIDTH, 0.0f), Point(BOARD_WIDTH, GOAL_TOP_OFFSET), Vector(-1.0f, 0.0f)), // Top right
		BoardWall(Point(BOARD_WIDTH, GOAL_BOTTOM_OFFSET), Point(BOARD_WIDTH, BOARD_HEIGHT), Vector(-1.0f, 0.0f)) // Bottom right
	};

	UniqueGameObject gameObject = std::make_unique<GameObject>(PUCK_ID);
	gameObject->AddComponent(std::make_unique<PositionComponent>(center));
	gameObject->AddComponent(std::make_unique<RadiusComponent>(radius));
	gameObject->AddComponent(std::make_unique<VelocityComponent>(Vector(0.0f, 0.0f)));
	gameObject->AddComponent(std::make_unique<PuckMovement>());
	gameObject->AddComponent(std::make_unique<PuckRenderer>(mRenderer, mResourceManager));
	gameObject->AddComponent(std::make_unique<PuckPhysics>(std::move(boardWalls)));

	return gameObject;
}

UniqueGameObject PlayGameState::CreateScoreUI(const Point& topLeft, int width, int height)
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(SCORE_UI_ID);
	gameObject->AddComponent(std::make_unique<ScoreUIData>(0, 0));
	gameObject->AddComponent(std::make_unique<BoxComponent>(topLeft, width, height));
	gameObject->AddComponent(std::make_unique<ScoreUILogic>());
	gameObject->AddComponent(std::make_unique<ScoreUIRenderer>(mRenderer, mResourceManager));
	
	return gameObject;
}

UniqueGameObject PlayGameState::CreateTimerUI(const Point& topLeft, int width, int height)
{
	UniqueGameObject gameObject = std::make_unique<GameObject>(TIMER_UI_ID);
	gameObject->AddComponent(std::make_unique<TimerUIData>(PLAY_TIME));
	gameObject->AddComponent(std::make_unique<BoxComponent>(topLeft, width, height));
	gameObject->AddComponent(std::make_unique<TimerUILogic>());
	gameObject->AddComponent(std::make_unique<TimerUIRenderer>(mRenderer, mResourceManager));

	return gameObject;
}