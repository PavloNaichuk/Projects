#pragma once

#include "PhysicsComponent.h"
#include "Helpers.h"

class PuckPhysics : public PhysicsComponent 
{
public:
	PuckPhysics(std::vector<BoardWall> boardWalls);

	ComponentId GetId() const override;
	void Update(GameObject& gameObject, GameObjectList& gameObjectList) override;

private: 
	std::vector<BoardWall> mBoardWalls;
};
