#include "EntityManager.h"
#include <algorithm>

void EntityManager::AddEntity(TriggerPointer trigger)
{
	m_Triggers.emplace_back(trigger);
}

void EntityManager::RemoveEntity(TriggerPointer trigger)
{
	auto it = std::remove(m_Triggers.begin(), m_Triggers.end(), trigger);
	m_Triggers.erase(it, m_Triggers.end());
}

void EntityManager::AddEntity(MovingEntityPointer entity)
{
	m_MovingEntities.emplace_back(entity);
}

void EntityManager::RemoveEntity(MovingEntityPointer entity)
{
	auto it = std::remove(m_MovingEntities.begin(), m_MovingEntities.end(), entity);
	m_MovingEntities.erase(it, m_MovingEntities.end());
}

void EntityManager::AddEntity(StaticEntityPointer entity)
{
	m_StaticEntities.emplace_back(entity);
}

void EntityManager::RemoveEntity(StaticEntityPointer entity)
{
	auto it = std::remove(m_StaticEntities.begin(), m_StaticEntities.end(), entity);
	m_StaticEntities.erase(it, m_StaticEntities.end());
}

void EntityManager::Clear()
{
	m_Triggers.clear();
	m_MovingEntities.clear();
	m_StaticEntities.clear();
}
