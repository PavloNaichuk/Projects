#include "RunAwayGoal.h"
#include "GameEntities/PacMan.h"
#include "PathFinding/PathFinder.h"
#include "PathFinding/Path.h"
#include "GameEntities/Ghost.h"

RunAwayGoal::RunAwayGoal(std::shared_ptr<PathFinder> pathFinder, const Vector2f& runAwayDest, const PacMan& pacMan)
	: m_PathFinder(pathFinder)
	, m_RunAwayDest(runAwayDest)
	, m_PacMan(pacMan)
{
}

void RunAwayGoal::Respawn(Ghost& ghost)
{
}

void RunAwayGoal::Activate(Ghost& ghost)
{
	m_PathToRunAwayDest = m_PathFinder->FindPath(ghost.GetCenter(), m_RunAwayDest);
	ghost.FollowPath(m_PathToRunAwayDest);
}

void RunAwayGoal::Update(Ghost& ghost)
{
}

Priority RunAwayGoal::EvaluatePriority(const Ghost& ghost) const
{
	return (m_PacMan.GetBehaviorMode() == BehaviorMode::POWERED) ? HIGH : LOW;
}