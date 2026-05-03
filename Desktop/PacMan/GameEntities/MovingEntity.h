#pragma once

#include "Math/Vector2f.h"
#include "GameEntities/BaseEntity.h"
#include "EntityStates/EntityState.h"
#include <memory>

enum Heading
{
	DOWN = 0,
	LEFT,
	RIGHT,
	UP,
	NUM_HEADINGS
};

class Path;
class EntityStateMachine;
class CollisionDetector;

class MovingEntity : public BaseEntity
{
public:
	MovingEntity(EntityType type, const Vector2f& center, const Vector2f& halfSize,
		Heading heading, float speed, std::shared_ptr<CollisionDetector> collisionDetector);

	virtual void Kill() = 0;
	virtual void Respawn(const Vector2f& center, Heading heading) = 0;

	void Update(std::time_t elapsedTimeInMS) override;
		
	Heading GetHeading() const { return m_Heading; }
	void SetHeading(Heading heading) { m_Heading = heading; }

	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }
	
	EntityStateType GetCurrentState();

	void Move();
	void Stop();

	void FollowPath(std::shared_ptr<Path> path);
	std::shared_ptr<Path> GetPath() { return m_Path; }
		
private:
	Heading m_Heading;
	float m_Speed;
	std::shared_ptr<EntityStateMachine> m_StateMachine;
	std::shared_ptr<Path> m_Path;
};

Heading ExtractHeading(const Vector2f& headingVector);
const Vector2f& ExtractHeadingVector(Heading heading);

bool Overlap(const MovingEntity& entity1, const MovingEntity& entity2);
