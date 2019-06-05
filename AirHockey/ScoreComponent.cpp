#include "pch.h"
#include "ScoreComponent.h"

void ScoreComponent::Set(unsigned score)
{
	mScore = score;
}

unsigned ScoreComponent::Get()
{
	return mScore;
}
