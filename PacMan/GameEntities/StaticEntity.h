#pragma once

#include "BaseEntity.h"

class StaticEntity : public BaseEntity
{
public:
	StaticEntity(EntityType type, const Vector2f& center, const Vector2f& halfSize);
	void Update(std::time_t /*elapsedTimeInMS*/) override {}
};