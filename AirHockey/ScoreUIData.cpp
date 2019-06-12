#include "pch.h"
#include "ScoreUIData.h"

ScoreUIData::ScoreUIData(int playerScore, int enemyScore)
	: mPlayerScore(playerScore)
	, mEnemyScore(enemyScore)
{
}

Component::ComponentId ScoreUIData::GetId() const
{
	return COMPONENT_ID;
}

int ScoreUIData::GetPlayerScore()
{
	return mPlayerScore;
}

void ScoreUIData::SetPlayerScore(int playerScore)
{
	mPlayerScore = playerScore;
}

int ScoreUIData::GetEnemyScore()
{
	return mEnemyScore;
}

void ScoreUIData::SetEnemyScore(int enemyScore)
{
	mEnemyScore = enemyScore;
}
