#pragma once

#include "Trigger.h"
#include "MovingEntity.h"
#include "StaticEntity.h"
#include <vector>
#include <memory>

class Trigger;

class EntityManager
{
public:
	using TriggerPointer = std::shared_ptr<Trigger>;
	using MovingEntityPointer = std::shared_ptr<MovingEntity>;
	using StaticEntityPointer = std::shared_ptr<StaticEntity>;

	using Triggers = std::vector<TriggerPointer>;
	using MovingEntities = std::vector<MovingEntityPointer>;
	using StaticEntities = std::vector<StaticEntityPointer>;

	EntityManager() = default;
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator= (const EntityManager&) = delete;

	Triggers& GetTriggers() { return m_Triggers; }
	void AddEntity(TriggerPointer trigger);
	void RemoveEntity(TriggerPointer trigger);
	
	MovingEntities& GetMovingEntities() { return m_MovingEntities; }
	void AddEntity(MovingEntityPointer entity);
	void RemoveEntity(MovingEntityPointer entity);

	StaticEntities& GetStaticEntities() { return m_StaticEntities; }
	void AddEntity(StaticEntityPointer entity);
	void RemoveEntity(StaticEntityPointer entity);
	
	void Clear();
			
private:
	Triggers m_Triggers;
	MovingEntities m_MovingEntities;
	StaticEntities m_StaticEntities;
};