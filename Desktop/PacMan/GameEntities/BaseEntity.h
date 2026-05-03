#pragma once

#include "Math/Vector2f.h"
#include <ctime>

enum class EntityType
{
	WALL,
	PEN_DOOR,
	PACMAN,
	BLINKY,
	PINKY,
	INKY,
	CLYDE,
	PILL_POINT,
	POWER_UP,
	TELEPORT
};

class BaseEntity
{
protected:
	BaseEntity(EntityType type, const Vector2f& center, const Vector2f& halfSize);

public:
	virtual ~BaseEntity() = default;
	virtual void Update(std::time_t elapsedTimeInMS) = 0;

	EntityType GetType() const { return m_Type; }

	const Vector2f& GetCenter() const;
	void SetCenter(const Vector2f& center);

	const Vector2f& GetHalfSize() const;
	void SetHalfSize(const Vector2f& halfSize);
	
private:
	EntityType m_Type;
	Vector2f m_Center;
	Vector2f m_HalfSize;
};