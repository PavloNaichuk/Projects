#pragma once

#include "AIGoal.h"
#include "Math/Vector2f.h"
#include <memory>

class PacMan;
class PathFinder;
class Path;

class RunAwayGoal : public AIGoal
{
public:
	RunAwayGoal(std::shared_ptr<PathFinder> pathFinder, const Vector2f& runAwayDest, const PacMan& pacMan);

	void Respawn(Ghost& ghost) override;
	void Activate(Ghost& ghost) override;
	void Update(Ghost& ghost) override;
	Priority EvaluatePriority(const Ghost& ghost) const override;

private:
	std::shared_ptr<PathFinder> m_PathFinder;
	Vector2f m_RunAwayDest;
	std::shared_ptr<Path> m_PathToRunAwayDest;
	const PacMan& m_PacMan;
};
