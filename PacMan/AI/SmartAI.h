#pragma once

#include "GhostAI.h"
#include <vector>
#include <memory>

class AIGoal;
class PathFinder;
class PacMan;
struct Vector2f;

class SmartAI : public GhostAI
{
public:
	SmartAI(std::shared_ptr<PathFinder> pathFinder, const Vector2f& runAwayDest, PacMan& pacMan);
	void Update(Ghost& ghost) override;
	void Respawn(Ghost& ghost) override;

private:
	using GoalPointer = std::shared_ptr<AIGoal>;
	GoalPointer EvaluateNextGoal(const Ghost& ghost);

private:
	std::vector<GoalPointer> m_Goals;
	GoalPointer m_CurrGoal;
};