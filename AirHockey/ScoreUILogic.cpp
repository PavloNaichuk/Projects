#include "pch.h"
#include "ScoreUILogic.h"

ScoreUILogic::ScoreUILogic(unsigned score)
	: mScore(score)
{
}

float ScoreUILogic::GetScore()
{
	return mScore;
}

void ScoreUILogic::SetTScore(unsigned score)
{
	mScore = score;
}

Component::ComponentId ScoreUILogic::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void ScoreUILogic::Update(GameObject& gameObject, float deltaTime)
{

}