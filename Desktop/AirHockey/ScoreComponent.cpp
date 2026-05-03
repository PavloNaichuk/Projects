#include "pch.h"
#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(unsigned score)
	: mScore(score)
{
}

Component::ComponentId ScoreComponent::GetId() const 
{
	return COMPONENT_ID;
}

void ScoreComponent::Set(unsigned score)
{
	mScore = score;
}

unsigned ScoreComponent::Get() const
{
	return mScore;
}
