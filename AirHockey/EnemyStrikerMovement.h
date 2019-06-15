#pragma once

#include "AIComponent.h"
#include "Helpers.h"

class EnemyStrikerMovement : public AIComponent
{
public:
	class State;
	using UniqueState = std::unique_ptr<State>;

	EnemyStrikerMovement(const Region& movementRegion);
	~EnemyStrikerMovement();

	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) override;
	
	void Reset();
	const Region& GetMovementRegion() const;
	void Enter(UniqueState state);

private:
	Region mMovemenetRegion;
	UniqueState mState;
	UniqueState mPendingState;
};
