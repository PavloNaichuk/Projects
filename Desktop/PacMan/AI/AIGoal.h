#pragma once

class Ghost;

enum Priority
{
	HIGH = 0,
	LOW
};

class AIGoal
{
public:
	virtual ~AIGoal() {}

	virtual void Respawn(Ghost& ghost) = 0;
	virtual void Activate(Ghost& ghost) = 0;
	virtual void Update(Ghost& ghost) = 0;
	virtual Priority EvaluatePriority(const Ghost& ghost) const = 0;
};