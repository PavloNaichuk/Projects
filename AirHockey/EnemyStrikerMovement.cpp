#include "pch.h"
#include "EnemyStrikerMovement.h"
#include "GameObject.h"
#include "Config.h"
#include "Math.h"
#include "VelocityComponent.h"
#include "PositionComponent.h"
#include "RadiusComponent.h"

class EnemyStrikerMovement::State 
{
public:
	virtual ~State() {}
	virtual void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) = 0;
};

class ReturnToPositionState : public EnemyStrikerMovement::State
{
public:
	ReturnToPositionState(GameObjectList& gameObjectList);
	void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) override;
	
private:
	Point mReturnPosition;
};

class KickPuckState : public EnemyStrikerMovement::State
{
public:
	KickPuckState();
	void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) override;

private:
	float mTimeAlreadyWaited;
	float mTimeToWait;
};

class GoToKickPositionState : public EnemyStrikerMovement::State 
{
public:
	GoToKickPositionState(GameObject& striker, GameObject& puck);
	void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) override;
	
private:
	Point mKickPosition;
};

class DefendState : public EnemyStrikerMovement::State 
{
public:
	void Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList) override;
};

EnemyStrikerMovement::EnemyStrikerMovement(const Region& movementRegion)
	: mMovemenetRegion(movementRegion)
{
	Reset();
}

EnemyStrikerMovement::~EnemyStrikerMovement() 
{
}

Component::ComponentId EnemyStrikerMovement::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void EnemyStrikerMovement::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	if (mPendingState)
	{
		mState = std::move(mPendingState);
		mPendingState = nullptr;
	}
	mState->Update(gameObject, deltaTime, gameObjectList);
}

void EnemyStrikerMovement::Reset() 
{
	Enter(std::make_unique<DefendState>());
}

const Region& EnemyStrikerMovement::GetMovementRegion() const 
{
	return mMovemenetRegion;
}

void EnemyStrikerMovement::Enter(UniqueState state) 
{
	mPendingState = std::move(state);
}

ReturnToPositionState::ReturnToPositionState(GameObjectList& gameObjectList)
{
	GameObject& goal = *gameObjectList[ENEMY_GOAL_ID];
	GameObject& striker = *gameObjectList[ENEMY_STRIKER_ID];

	PositionComponent* goalPositionComponent = goal.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	const Point& goalCenter = goalPositionComponent->GetCenter();

	RadiusComponent* strikerRadiusComponent = striker.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);
	mReturnPosition = goalCenter + Vector(strikerRadiusComponent->GetRadius(), 0.0f);
}

void ReturnToPositionState::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	PositionComponent* strikerPositionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* strikerVelocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	EnemyStrikerMovement* strikerMovementComponent = gameObject.GetComponent<EnemyStrikerMovement>(EnemyStrikerMovement::COMPONENT_ID);

	const Point& strikerCenter = strikerPositionComponent->GetCenter();
	const float distToTarget = Distance(strikerCenter, mReturnPosition);

	if (AreEqual(distToTarget, 0.0f))
	{
		strikerVelocityComponent->Set(Vector(0.0f, 0.0f));
		strikerMovementComponent->Enter(std::make_unique<DefendState>());
	}
	else
	{
		const Vector moveDir = Normalize(mReturnPosition - strikerCenter);
		const Vector strikerVelocity = STRIKER_SPEED * moveDir;

		Point strikerNewCenter = strikerCenter + deltaTime * strikerVelocity;
		float distWillGo = Distance(strikerCenter, strikerNewCenter);

		if (distWillGo > distToTarget)
		{
			strikerNewCenter = mReturnPosition;
		}

		strikerPositionComponent->SetCenter(strikerNewCenter);
		strikerVelocityComponent->Set(strikerVelocity);
	}
}

KickPuckState::KickPuckState()
	: mTimeAlreadyWaited(0.0f)
	, mTimeToWait(Random(ENEMY_MIN_WAIT_KICK_TIME_IN_SEC, ENEMY_MAX_WAIT_KICK_TIME_IN_SEC))
{
}

void KickPuckState::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	if (mTimeAlreadyWaited > mTimeToWait)
	{
		GameObject& goal = *gameObjectList[PLAYER_GOAL_ID];
		GameObject& puck = *gameObjectList[PUCK_ID];

		EnemyStrikerMovement* strikerMovementComponent = gameObject.GetComponent<EnemyStrikerMovement>(EnemyStrikerMovement::COMPONENT_ID);

		PositionComponent* goalPositionComponent = goal.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
		RadiusComponent* goalRadiusComponent = goal.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

		const Point& goalCenter = goalPositionComponent->GetCenter();

		PositionComponent* puckPositionComponent = puck.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
		VelocityComponent* puckVelocityComponent = puck.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);

		const Point& puckCenter = puckPositionComponent->GetCenter();

		const Vector postOffset(0.0f, goalRadiusComponent->GetRadius());
		const Point topPost = goalCenter - postOffset;
		const Point bottomPost = goalCenter + postOffset;

		float kickDir = Random(0.0f, 1.0f);
		Point kickTarget = topPost + kickDir * (bottomPost - topPost);
		Vector puckMoveDir = Normalize(kickTarget - puckCenter);

		float puckSpeed = Random(0.5f * PUCK_SPEED, PUCK_SPEED);
		puckVelocityComponent->Set(puckSpeed * puckMoveDir);

		strikerMovementComponent->Enter(std::make_unique<ReturnToPositionState>(gameObjectList));
	}
	else
	{
		mTimeAlreadyWaited += deltaTime;
	}
}

GoToKickPositionState::GoToKickPositionState(GameObject& striker, GameObject& puck)
{
	PositionComponent* puckPositionComponent = puck.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	RadiusComponent* puckRadiusComponent = puck.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);

	const Point& puckCenter = puckPositionComponent->GetCenter();
	const float puckRadius = puckRadiusComponent->GetRadius();

	RadiusComponent* strikerRadiusComponent = striker.GetComponent<RadiusComponent>(RadiusComponent::COMPONENT_ID);
	const float strikerRadius = strikerRadiusComponent->GetRadius();

	mKickPosition = puckCenter - Vector(puckRadius + strikerRadius, 0.0f);
}

void GoToKickPositionState::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	PositionComponent* strikerPositionComponent = gameObject.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* strikerVelocityComponent = gameObject.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);
	EnemyStrikerMovement* strikerMovement = gameObject.GetComponent<EnemyStrikerMovement>(EnemyStrikerMovement::COMPONENT_ID);

	const Point& strikerCenter = strikerPositionComponent->GetCenter();
	const float distToKickPosition = Distance(strikerCenter, mKickPosition);

	if (AreEqual(distToKickPosition, 0.0f))
	{
		strikerVelocityComponent->Set(Vector(0.0f, 0.0f));
		strikerMovement->Enter(std::make_unique<KickPuckState>());
	}
	else
	{
		const Vector moveDir = Normalize(mKickPosition - strikerCenter);

		Vector strikerVelocity = STRIKER_SPEED * moveDir;
		Point strikerNewCenter = strikerCenter + deltaTime * strikerVelocity;

		const float distWillGo = Distance(strikerCenter, strikerNewCenter);
		if (distWillGo > distToKickPosition)
		{
			strikerNewCenter = mKickPosition;
		}

		strikerPositionComponent->SetCenter(strikerNewCenter);
		strikerVelocityComponent->Set(strikerVelocity);
	}
}

void DefendState::Update(GameObject& gameObject, float deltaTime, GameObjectList& gameObjectList)
{
	GameObject& puck = *gameObjectList[PUCK_ID];

	EnemyStrikerMovement* strikerMovementComponent = gameObject.GetComponent<EnemyStrikerMovement>(EnemyStrikerMovement::COMPONENT_ID);
	const Region& strikerMovementRegion = strikerMovementComponent->GetMovementRegion();

	PositionComponent* puckPositionComponent = puck.GetComponent<PositionComponent>(PositionComponent::COMPONENT_ID);
	VelocityComponent* puckVelocityComponent = puck.GetComponent<VelocityComponent>(VelocityComponent::COMPONENT_ID);

	const Point& puckCenter = puckPositionComponent->GetCenter();
	const Vector& puckVelocity = puckVelocityComponent->Get();

	if ((strikerMovementRegion.mTopLeft.mX < puckCenter.mX) && (puckCenter.mX < strikerMovementRegion.mBottonRight.mX))
	{
		if (IsZero(puckVelocity))
		{
			strikerMovementComponent->Enter(std::make_unique<GoToKickPositionState>(gameObject, puck));
		}
	}
}