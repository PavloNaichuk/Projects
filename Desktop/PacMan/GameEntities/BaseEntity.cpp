#include "BaseEntity.h"

BaseEntity::BaseEntity(EntityType type, const Vector2f& center, const Vector2f& halfSize)
	: m_Type(type)
	, m_Center(center)
	, m_HalfSize(halfSize)
{}

const Vector2f& BaseEntity::GetCenter() const
{
	return m_Center;
}

void BaseEntity::SetCenter(const Vector2f& center)
{
	m_Center = center;
}

const Vector2f& BaseEntity::GetHalfSize() const
{
	return m_HalfSize;
}

void BaseEntity::SetHalfSize(const Vector2f& halfSize)
{
	m_HalfSize = halfSize;
}
