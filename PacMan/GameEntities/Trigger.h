#pragma once

#include "BaseEntity.h"

class PacMan;

class Trigger : public BaseEntity
{
public:
	Trigger(EntityType type, const Vector2f& center, const Vector2f& halfSize, bool active)
		: BaseEntity(type, center, halfSize)
		, m_Active(active)
	{}

	bool IsActive() const { return m_Active; }
	void Activate() { m_Active = true; }
	void Deactivate() { m_Active = false; }

	virtual void Try(PacMan& pacMan) = 0;
	virtual bool IsTouching(const PacMan& pacMan) const = 0;

private:
	bool m_Active;
};
