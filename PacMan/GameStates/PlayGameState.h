#pragma once

#include "GameState.h"
#include "Core/MessageCenter.h"
#include "GameEntities/BaseEntity.h"
#include <memory>
#include <vector>
#include <cstdlib>

class LevelMap;
class RenderSystem;
class GameRenderer;
class EntityManager;
class PathFinder;
class CollisionDetector;
class PacMan;
class Ghost;
struct Vector2f;

class PlayGameState : public GameState, public MessageListener
{
public:
	PlayGameState(std::shared_ptr<LevelMap> levelMap, std::shared_ptr<RenderSystem> renderSystem);
	PlayGameState(const PlayGameState&) = delete;
	PlayGameState& operator= (const PlayGameState&) = delete;

	void Enter() override;
	void Update(std::time_t elapsedTimeInMS) override;
	void Exit() override;
	void Render() override;
	void ProcessKeyPress(char key) override;
	void ProcessKeyRelease(char key) override;

private:
	void OnMessageReceived(const Message& message) override;
	
	void SetupEntities(Vector2f* pOutWorldMinPoint = nullptr, Vector2f* pOutWorldMaxPoint = nullptr);
	void RespawnEntity(EntityType entityType);

private:
	std::shared_ptr<LevelMap> m_LevelMap;
	std::shared_ptr<PathFinder> m_PathFinder;
	std::shared_ptr<EntityManager> m_EntityManager;
	std::shared_ptr<GameRenderer> m_Renderer;
	std::shared_ptr<CollisionDetector> m_CollisionDetector;
	std::shared_ptr<PacMan> m_PacMan;
	std::shared_ptr<Ghost> m_Blinky;
	std::shared_ptr<Ghost> m_Pinky;
	std::size_t m_NumCollectedResources;
	std::size_t m_NumResourcesToCollect;
};
