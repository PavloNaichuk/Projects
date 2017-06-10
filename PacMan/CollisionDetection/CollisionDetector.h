#pragma once

#include "GameEntities/MovingEntity.h"
#include <memory>
#include <array>
#include <vector>
#include <functional>

class LevelMap;
class MovingEntity;
class EntityManager;
struct Vector2f;

struct Collision
{
	Collision(float penetration = 0.0f)
		: m_Penetration(penetration)
	{}
	float m_Penetration;
};

enum CollisionCheckFlags
{
	CheckVersusStaticEntities = 1 << 0,
	CheckVersusGhosts = 1 << 1
};

class CollisionDetector
{
public:
	CollisionDetector(std::shared_ptr<LevelMap> levelMap, std::shared_ptr<EntityManager> entityManager);
	void DetectNearestCollision(const MovingEntity& entity, const Vector2f& entityHeadingVector, float testDist, Collision* pCollision, unsigned checkFlags);

private:
	void PopulateStaticEntityCollisionChecks();
	void PopulateGhostCollisionChecks();
	void PopulateFeelersGenerators();

private:
	using Feelers = std::vector<Vector2f>;
	using FeelersGenerator = std::function<Feelers(const MovingEntity& entity)>;

	using StaticEntityCollisionCheck = std::function<float (const Vector2f& entityCenter, const Vector2f& feelerPoint)>;
	using GhostCollisionCheck = std::function<float (const MovingEntity& ghost, const Vector2f& feelerStart, const Vector2f& feelerEnd)>;

	std::shared_ptr<LevelMap> m_LevelMap;
	std::shared_ptr<EntityManager> m_EntityManager;

	std::array<FeelersGenerator, NUM_HEADINGS> mFeelersGenerators;
	std::array<StaticEntityCollisionCheck, NUM_HEADINGS> mStaticEntityCollisionChecks;
	std::array<GhostCollisionCheck, NUM_HEADINGS> mGhostCollisionChecks;
};