#include "SmartAI.h"
#include "ChaseGoal.h"
#include "RunAwayGoal.h"
#include <algorithm>
#include <cassert>

SmartAI::SmartAI(std::shared_ptr<PathFinder> pathFinder, const Vector2f& runAwayDest, PacMan& pacMan)
{
	m_Goals.emplace_back(std::make_shared<ChaseGoal>(pathFinder, pacMan));
	m_Goals.emplace_back(std::make_shared<RunAwayGoal>(pathFinder, runAwayDest, pacMan));
}

void SmartAI::Update(Ghost& ghost)
{
	auto nextGoal = EvaluateNextGoal(ghost);
	if (m_CurrGoal != nextGoal)
	{
		m_CurrGoal = nextGoal;
		m_CurrGoal->Activate(ghost);
	}
	else
	{
		m_CurrGoal->Update(ghost);
	}
}

void SmartAI::Respawn(Ghost& ghost)
{
	for (auto goal : m_Goals)
		goal->Respawn(ghost);
}

SmartAI::GoalPointer SmartAI::EvaluateNextGoal(const Ghost& ghost)
{
	auto hasHigherPriorityCheck = [&ghost](GoalPointer goal1, GoalPointer goal2)
	{
		return (goal1->EvaluatePriority(ghost) < goal2->EvaluatePriority(ghost));
	};

	auto it = std::min_element(m_Goals.begin(), m_Goals.end(), hasHigherPriorityCheck);
	assert(it != m_Goals.end());
	return *it;
}