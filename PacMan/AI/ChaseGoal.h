#pragma once

#include "AIGoal.h"
#include <memory>
#include <ctime>

class Path;
class PathFinder;
class PacMan;

class ChaseGoal : public AIGoal
{
public:
	ChaseGoal(std::shared_ptr<PathFinder> pathFinder, PacMan& pacMan);

	void Respawn(Ghost& ghost) override;
	void Activate(Ghost& ghost) override;
	void Update(Ghost& ghost) override;
	Priority EvaluatePriority(const Ghost& ghost) const override;

private:
	std::shared_ptr<PathFinder> m_PathFinder;
	std::shared_ptr<Path> m_PathToPacMan;
	std::time_t m_PathTimeStampInSec;
	PacMan& m_PacMan;
};