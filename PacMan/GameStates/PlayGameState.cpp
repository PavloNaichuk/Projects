#include "PlayGameState.h"
#include "AI/SmartAI.h"
#include "Core/LevelMap.h"
#include "Core/Config.h"
#include "Math/Vector2f.h"
#include "PathFinding/PathFinder.h"
#include "CollisionDetection/CollisionDetector.h"
#include "GameEntities/EntityManager.h"
#include "GameEntities/PacMan.h"
#include "GameEntities/Ghost.h"
#include "GameEntities/StaticEntity.h"
#include "GameEntities/PowerUp.h"
#include "GameEntities/PillPoint.h"
#include "GameEntities/Teleport.h"
#include "Render/GameRenderer.h"
#include <algorithm>
#include <cassert>

PlayGameState::PlayGameState(std::shared_ptr<LevelMap> levelMap, std::shared_ptr<RenderSystem> renderSystem)
	: m_LevelMap(levelMap)
	, m_PathFinder(new PathFinder(levelMap))
	, m_EntityManager(new EntityManager())
	, m_Renderer(new GameRenderer(renderSystem))
	, m_CollisionDetector(new CollisionDetector(levelMap, m_EntityManager))
	, m_NumCollectedResources(0)
	, m_NumResourcesToCollect(0)
{
}

void PlayGameState::Enter()
{
	MessageCenter::GetInstance().RegisterListener(this);

	Vector2f worldMinPoint, worldMaxPoint;
	SetupEntities(&worldMinPoint, &worldMaxPoint);

	m_Renderer->Setup(m_EntityManager, worldMinPoint, worldMaxPoint);
}

void PlayGameState::Update(std::time_t elapsedTimeInMS)
{
	for (auto movingEntity : m_EntityManager->GetMovingEntities())
		movingEntity->Update(elapsedTimeInMS);
	
	for (auto trigger : m_EntityManager->GetTriggers())
		trigger->Update(elapsedTimeInMS);
	for (auto trigger : m_EntityManager->GetTriggers())
		trigger->Try(*m_PacMan);
	
	m_Renderer->Update();
}

void PlayGameState::Exit()
{
	MessageCenter::GetInstance().UnregisterListener(this);
}

void PlayGameState::Render()
{
	m_Renderer->Draw();
}

void PlayGameState::ProcessKeyPress(char key)
{
	if (key == VK_LEFT)
	{
		m_PacMan->SetHeading(LEFT);
		m_PacMan->Move();
		return;
	}
	if (key == VK_RIGHT)
	{
		m_PacMan->SetHeading(RIGHT);
		m_PacMan->Move();
		return;
	}
	if (key == VK_UP)
	{
		m_PacMan->SetHeading(UP);
		m_PacMan->Move();
		return;
	}
	if (key == VK_DOWN)
	{
		m_PacMan->SetHeading(DOWN);
		m_PacMan->Move();
		return;
	}
}

void PlayGameState::ProcessKeyRelease(char key)
{
	if ((key == VK_LEFT) || (key == VK_RIGHT) || (key == VK_UP) || (key == VK_DOWN))
		m_PacMan->Stop();
}

void PlayGameState::OnMessageReceived(const Message& message)
{
	if ((message.m_ID == PowerUpChanged) || (message.m_ID == PillPointChanged))
	{
		auto pTrigger = (Trigger*)message.m_pSender;
		if (!pTrigger->IsActive())
		{
			++m_NumCollectedResources;
			if (m_NumResourcesToCollect == m_NumCollectedResources)
			{
				MessageCenter::GetInstance().AddMessage({LevelCompleted, this, nullptr});
			}
		}
	}
	else if (message.m_ID == EntityKilled)
	{
		auto pEntity = (MovingEntity*)message.m_pSender;
		if (pEntity->GetType() == EntityType::PACMAN)
		{
			if (m_PacMan->GetLives() == 0)
			{
				MessageCenter::GetInstance().AddMessage({LevelFailed, this, nullptr});
			}
			else
			{
				RespawnEntity(EntityType::PACMAN);
				RespawnEntity(EntityType::BLINKY);
				RespawnEntity(EntityType::PINKY);
			}
		}
		else
		{
			m_PacMan->AddScore(Config::KILLING_GHOST_SCORE);
			RespawnEntity(pEntity->GetType());
		}
	}
}

void PlayGameState::SetupEntities(Vector2f* pOutWorldMinPoint, Vector2f* pOutWorldMaxPoint)
{
	m_NumCollectedResources = 0;
	m_NumResourcesToCollect = 0;

	const Vector2f worldSpaceTileSize(Config::MAP_TILE_WORLD_SIZE);
	const Vector2f worldSpaceTileHalfSize(Config::MAP_TILE_WORLD_HALF_SIZE);

	for (int row = 0; row < Config::NUM_MAP_TILE_ROWS; ++row)
	{
		for (int col = 0; col < Config::NUM_MAP_TILE_COLS; ++col)
		{
			const MapTile& mapTile = m_LevelMap->GetTile(row, col);
			const Vector2f worldSpaceTileCenter = CalcMapTileCenter(row, col);

			if (mapTile.m_MapItemType == MapItemType::PEN_DOOR)
				m_EntityManager->AddEntity(std::make_shared<StaticEntity>(EntityType::PEN_DOOR, worldSpaceTileCenter, worldSpaceTileHalfSize));
			else if (mapTile.m_MapItemType == MapItemType::WALL)
				m_EntityManager->AddEntity(std::make_shared<StaticEntity>(EntityType::WALL, worldSpaceTileCenter, worldSpaceTileHalfSize));
			else if (mapTile.m_MapItemType == MapItemType::POWER_UP)
			{
				m_EntityManager->AddEntity(std::make_shared<PowerUp>(worldSpaceTileCenter, worldSpaceTileHalfSize, true, Config::POWER_UP_SCORE));
				++m_NumResourcesToCollect;
			}
			else if (mapTile.m_MapItemType == MapItemType::POINT_PILL)
			{
				m_EntityManager->AddEntity(std::make_shared<PillPoint>(worldSpaceTileCenter, worldSpaceTileHalfSize, true, Config::PILL_POINT_SCORE));
				++m_NumResourcesToCollect;
			}
		}
	}
	{
		const MapTile& teleportTile1 = m_LevelMap->GetTeleportTile1();
		const MapTile& teleportTile2 = m_LevelMap->GetTeleportTile2();

		const Vector2f worldSpaceCenter1 = CalcMapTileCenter(teleportTile1.m_Row, teleportTile1.m_Col);
		const Vector2f worldSpaceCenter2 = CalcMapTileCenter(teleportTile2.m_Row, teleportTile2.m_Col);

		const Vector2f worldSpaceHalfSize = worldSpaceTileHalfSize - 3.0f;

		auto teleport1 = std::make_shared<Teleport>(worldSpaceCenter1, worldSpaceCenter1, worldSpaceCenter2);
		auto teleport2 = std::make_shared<Teleport>(worldSpaceCenter2, worldSpaceCenter2, worldSpaceCenter1);

		m_EntityManager->AddEntity(teleport1);
		m_EntityManager->AddEntity(teleport2);
	}
	{
		const MapTile& mapTile = m_LevelMap->GetPacManSpawnTile();
		const Vector2f worldSpaceCenter = CalcMapTileCenter(mapTile.m_Row, mapTile.m_Col);
		const Vector2f worldSpaceHalfSize = worldSpaceTileHalfSize - 3.0f;
		
		m_PacMan = std::make_shared<PacMan>(worldSpaceCenter, worldSpaceHalfSize, Heading::DOWN, Config::PACMAN_SPEED, Config::PACMAN_LIVES, m_CollisionDetector, m_EntityManager);
		m_EntityManager->AddEntity(m_PacMan);
	}
	{
		assert(m_PacMan);

		const MapTile& mapTile = m_LevelMap->GetBlinkySpawnTile();
		const Vector2f worldSpaceCenter = CalcMapTileCenter(mapTile.m_Row, mapTile.m_Col);
		const Vector2f worldSpaceHalfSize = worldSpaceTileHalfSize - 3.0f;

		auto blinkyAI = std::make_shared<SmartAI>(m_PathFinder, worldSpaceCenter, *m_PacMan);
		m_Blinky = std::make_shared<Ghost>(EntityType::BLINKY, worldSpaceCenter, worldSpaceHalfSize,
			Heading::DOWN, Config::BLINKY_SPEED, m_CollisionDetector, blinkyAI);

		m_EntityManager->AddEntity(m_Blinky);
	}
	{
		assert(m_PacMan);

		const MapTile& mapTile = m_LevelMap->GetPinkySpawnTile();
		const Vector2f worldSpaceCenter = CalcMapTileCenter(mapTile.m_Row, mapTile.m_Col);
		const Vector2f worldSpaceHalfSize = worldSpaceTileHalfSize - 3.0f;
		
		auto pinkyAI = std::make_shared<SmartAI>(m_PathFinder, worldSpaceCenter, *m_PacMan);
		m_Pinky = std::make_shared<Ghost>(EntityType::PINKY, worldSpaceCenter, worldSpaceHalfSize,
			Heading::DOWN, Config::PINKY_SPEED, m_CollisionDetector, pinkyAI);

		m_EntityManager->AddEntity(m_Pinky);
	}

	if (pOutWorldMinPoint != nullptr)
		*pOutWorldMinPoint = Vector2f(0.0f, -1.0f * Config::NUM_MAP_TILE_ROWS * worldSpaceTileSize.m_Y);
	
	if (pOutWorldMaxPoint != nullptr)
		*pOutWorldMaxPoint = Vector2f(Config::NUM_MAP_TILE_COLS * worldSpaceTileSize.m_X, 0.0f);
}

void PlayGameState::RespawnEntity(EntityType entityType)
{
	if (entityType == EntityType::PACMAN)
	{
		const MapTile& mapTile = m_LevelMap->GetPacManSpawnTile();
		const Vector2f worldSpaceCenter = CalcMapTileCenter(mapTile.m_Row, mapTile.m_Col);

		m_PacMan->Respawn(worldSpaceCenter, Heading::DOWN);
	}
	else if (entityType == EntityType::BLINKY)
	{
		const MapTile& mapTile = m_LevelMap->GetBlinkySpawnTile();
		const Vector2f worldSpaceCenter = CalcMapTileCenter(mapTile.m_Row, mapTile.m_Col);

		m_Blinky->Respawn(worldSpaceCenter, Heading::DOWN);
	}
	else if (entityType == EntityType::PINKY)
	{
		const MapTile& mapTile = m_LevelMap->GetPinkySpawnTile();
		const Vector2f worldSpaceCenter = CalcMapTileCenter(mapTile.m_Row, mapTile.m_Col);

		m_Pinky->Respawn(worldSpaceCenter, Heading::DOWN);
	}
	else
	{
		assert(false);
	}
}
