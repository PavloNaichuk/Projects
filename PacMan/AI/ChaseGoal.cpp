#include "ChaseGoal.h"
#include "PathFinding/PathFinder.h"
#include "PathFinding/Path.h"
#include "GameEntities/Ghost.h"
#include "GameEntities/PacMan.h"
#include "Core/Config.h"

ChaseGoal::ChaseGoal(std::shared_ptr<PathFinder> pathFinder, PacMan& pacMan)
	: m_PathFinder(std::move(pathFinder))
	, m_PathTimeStampInSec(0)
	, m_PacMan(pacMan)
{
}

void ChaseGoal::Respawn(Ghost& ghost)
{
	Activate(ghost);
}

void ChaseGoal::Activate(Ghost& ghost)
{
	m_PathToPacMan = m_PathFinder->FindPath(ghost.GetCenter(), m_PacMan.GetCenter());
	m_PathTimeStampInSec = std::time(nullptr);
	ghost.FollowPath(m_PathToPacMan);
}

void ChaseGoal::Update(Ghost& ghost)
{
	if (Overlap(ghost, m_PacMan))
	{
		m_PacMan.Kill();
		ghost.Stop();
		
		return;
	}
	if (ghost.GetCurrentState() == EntityStateType::FOLLOW_PATH)
	{
		const std::time_t currTimeInSec = std::time(nullptr);
		if (currTimeInSec > m_PathTimeStampInSec + Config::PATH_VALIDATION_TIMEOUT_IN_SEC)
		{
			m_PathToPacMan = m_PathFinder->FindPath(ghost.GetCenter(), m_PacMan.GetCenter());
			m_PathTimeStampInSec = currTimeInSec;
			ghost.FollowPath(m_PathToPacMan);
		}
		return;
	}
}

Priority ChaseGoal::EvaluatePriority(const Ghost& ghost) const
{
	return (m_PacMan.GetBehaviorMode() == BehaviorMode::NORMAL) ? HIGH : LOW;
}
