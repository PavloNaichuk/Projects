#include "CollisionDetector.h"
#include "Core/LevelMap.h"
#include "Core/Config.h"
#include "GameEntities/EntityManager.h"
#include <cassert>
#include <algorithm>
#include <cmath>

namespace
{
	bool BelongsToRange(float from, float value, float to);
}

CollisionDetector::CollisionDetector(std::shared_ptr<LevelMap> levelMap, std::shared_ptr<EntityManager> entityManager)
	: m_LevelMap(std::move(levelMap))
	, m_EntityManager(std::move(entityManager))
{
	PopulateStaticEntityCollisionChecks();
	PopulateGhostCollisionChecks();
	PopulateFeelersGenerators();
}

void CollisionDetector::PopulateStaticEntityCollisionChecks()
{
	mStaticEntityCollisionChecks[RIGHT] = [](const Vector2f& entityCenter, const Vector2f& feelerPoint)
	{
		float penetration = 0.0f;

		const float leftX = entityCenter.m_X - Config::MAP_TILE_WORLD_HALF_SIZE;
		if (feelerPoint.m_X >= leftX)
			penetration = 1.0f + feelerPoint.m_X - leftX;

		return penetration;
	};
	mStaticEntityCollisionChecks[LEFT] = [](const Vector2f& entityCenter, const Vector2f& feelerPoint)
	{
		float penetration = 0.0f;

		const float rightX = entityCenter.m_X + Config::MAP_TILE_WORLD_HALF_SIZE;
		if (feelerPoint.m_X <= rightX)
			penetration = 1.0f + rightX - feelerPoint.m_X;

		return penetration;
	};
	mStaticEntityCollisionChecks[DOWN] = [](const Vector2f& entityCenter, const Vector2f& feelerPoint)
	{
		float penetration = 0.0f;

		const float topY = entityCenter.m_Y + Config::MAP_TILE_WORLD_HALF_SIZE;
		if (feelerPoint.m_Y <= topY)
			penetration = 1.0f + topY - feelerPoint.m_Y;

		return penetration;
	};
	mStaticEntityCollisionChecks[UP] = [](const Vector2f& entityCenter, const Vector2f& feelerPoint)
	{
		float penetration = 0.0f;

		const float bottomY = entityCenter.m_Y - Config::MAP_TILE_WORLD_HALF_SIZE;
		if (feelerPoint.m_Y >= bottomY)
			penetration = 1.f + feelerPoint.m_Y - bottomY;

		return penetration;
	};
}

void CollisionDetector::PopulateGhostCollisionChecks()
{
	mGhostCollisionChecks[RIGHT] = [](const MovingEntity& ghost, const Vector2f& feelerStart, const Vector2f& feelerEnd)
	{
		const Vector2f minPoint = ghost.GetCenter() - ghost.GetHalfSize();
		const Vector2f maxPoint = ghost.GetCenter() + ghost.GetHalfSize();

		float penetration = 0.0f;
		if (BelongsToRange(feelerStart.m_X, minPoint.m_X, feelerEnd.m_X) && BelongsToRange(minPoint.m_Y, feelerEnd.m_Y, maxPoint.m_Y))
			penetration = 1.0f + feelerEnd.m_X - minPoint.m_X;

		return penetration;
	};
	mGhostCollisionChecks[LEFT] = [](const MovingEntity& ghost, const Vector2f& feelerStart, const Vector2f& feelerEnd)
	{
		const Vector2f minPoint = ghost.GetCenter() - ghost.GetHalfSize();
		const Vector2f maxPoint = ghost.GetCenter() + ghost.GetHalfSize();

		float penetration = 0.0f;
		if (BelongsToRange(feelerEnd.m_X, maxPoint.m_X, feelerStart.m_X) && BelongsToRange(minPoint.m_Y, feelerEnd.m_Y, maxPoint.m_Y))
			penetration = 1.0f + maxPoint.m_X - feelerEnd.m_X;

		return penetration;
	};
	mGhostCollisionChecks[DOWN] = [](const MovingEntity& ghost, const Vector2f& feelerStart, const Vector2f& feelerEnd)
	{
		const Vector2f minPoint = ghost.GetCenter() - ghost.GetHalfSize();
		const Vector2f maxPoint = ghost.GetCenter() + ghost.GetHalfSize();

		float penetration = 0.0f;
		if (BelongsToRange(minPoint.m_X, feelerEnd.m_X, maxPoint.m_X) && BelongsToRange(feelerEnd.m_Y, maxPoint.m_Y, feelerStart.m_Y))
			penetration = 1.0f + feelerStart.m_Y - maxPoint.m_Y;

		return penetration;
	};
	mGhostCollisionChecks[UP] = [](const MovingEntity& ghost, const Vector2f& feelerStart, const Vector2f& feelerEnd)
	{
		const Vector2f minPoint = ghost.GetCenter() - ghost.GetHalfSize();
		const Vector2f maxPoint = ghost.GetCenter() + ghost.GetHalfSize();

		float penetration = 0.0f;
		if (BelongsToRange(minPoint.m_X, feelerEnd.m_X, maxPoint.m_X) && BelongsToRange(feelerStart.m_Y, minPoint.m_Y, feelerEnd.m_Y))
			penetration = 1.0f + feelerEnd.m_Y - minPoint.m_Y;

		return penetration;
	};
}

void CollisionDetector::PopulateFeelersGenerators()
{
	mFeelersGenerators[RIGHT] = [](const MovingEntity& entity) -> Feelers
	{
		const Vector2f& entityCenter = entity.GetCenter();
		const Vector2f& entityHalfSize = entity.GetHalfSize();
				
		return {Vector2f(entityCenter.m_X + entityHalfSize.m_X, entityCenter.m_Y + entityHalfSize.m_Y),
			Vector2f(entityCenter.m_X + entityHalfSize.m_X, entityCenter.m_Y - entityHalfSize.m_Y)};
	};
	mFeelersGenerators[LEFT] = [](const MovingEntity& entity) -> Feelers
	{
		const Vector2f& entityCenter = entity.GetCenter();
		const Vector2f& entityHalfSize = entity.GetHalfSize();

		return{ Vector2f(entityCenter.m_X - entityHalfSize.m_X, entityCenter.m_Y + entityHalfSize.m_Y),
			Vector2f(entityCenter.m_X - entityHalfSize.m_X, entityCenter.m_Y - entityHalfSize.m_Y)};
	};
	mFeelersGenerators[DOWN] = [](const MovingEntity& entity) -> Feelers
	{
		const Vector2f& entityCenter = entity.GetCenter();
		const Vector2f& entityHalfSize = entity.GetHalfSize();
		
		return {Vector2f(entityCenter.m_X + entityHalfSize.m_X, entityCenter.m_Y - entityHalfSize.m_Y),
			Vector2f(entityCenter.m_X - entityHalfSize.m_X, entityCenter.m_Y - entityHalfSize.m_Y)};
	};
	mFeelersGenerators[UP] = [](const MovingEntity& entity) -> Feelers
	{
		const Vector2f& entityCenter = entity.GetCenter();
		const Vector2f& entityHalfSize = entity.GetHalfSize();

		return {Vector2f(entityCenter.m_X + entityHalfSize.m_X, entityCenter.m_Y + entityHalfSize.m_Y),
			Vector2f(entityCenter.m_X - entityHalfSize.m_X, entityCenter.m_Y + entityHalfSize.m_Y)};
	};
}

void CollisionDetector::DetectNearestCollision(const MovingEntity& entity, const Vector2f& entityHeadingVector, float testDist, Collision* pCollision, unsigned checkFlags)
{
	assert(IsNormalized(entityHeadingVector));
	pCollision->m_Penetration = 0.0f;

	const Heading entityHeading = ExtractHeading(entityHeadingVector);
	
	auto feelersGenerator = mFeelersGenerators[entityHeading];
	auto staticEntityCollisionCheck = mStaticEntityCollisionChecks[entityHeading];
	auto ghostCollisionCheck = mGhostCollisionChecks[entityHeading];

	for (const Vector2f& feelerStart : feelersGenerator(entity))
	{
		const Vector2f feelerEnd = feelerStart + testDist * entityHeadingVector;
		if ((checkFlags & CheckVersusStaticEntities) != 0)
		{
			int startRow, startCol;
			CalcMapTileRowAndColumn(feelerStart, startRow, startCol);

			int endRow, endCol;
			CalcMapTileRowAndColumn(feelerEnd, endRow, endCol);

			const int numTiles = std::max(std::abs(endRow - startRow), std::abs(endCol - startCol));
			for (int i = 1; i <= numTiles; ++i)
			{
				int tileRow = startRow - i * int(entityHeadingVector.m_Y);
				int tileCol = startCol + i * int(entityHeadingVector.m_X);

				const MapTile& mapTile = m_LevelMap->GetTile(tileRow, tileCol);
				if (!IsTraversableForPacMan(mapTile))
				{
					const Vector2f tileCenter = CalcMapTileCenter(tileRow, tileCol);

					float penetration = staticEntityCollisionCheck(tileCenter, feelerEnd);
					if (penetration > pCollision->m_Penetration)
					{
						pCollision->m_Penetration = penetration;
						break;
					}
				}
			}
		}
		if ((checkFlags & CheckVersusGhosts) != 0)
		{
			for (auto otherEntity : m_EntityManager->GetMovingEntities())
			{
				if ((entity.GetType() != otherEntity->GetType()) && (otherEntity->GetType() != EntityType::PACMAN))
				{
					float penetration = ghostCollisionCheck(*otherEntity, feelerStart, feelerEnd);
					if (penetration > pCollision->m_Penetration)
					{
						pCollision->m_Penetration = penetration;
					}
				}
			}
		}
	}
}

namespace
{
	bool BelongsToRange(float from, float value, float to)
	{
		return !(value < from) && !(value > to);
	}
}