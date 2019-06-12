#pragma once

#include "Component.h"

class ScoreUIData : public Component
{
public:
	ScoreUIData(int playerScore, int enemyScore);

	static const ComponentId COMPONENT_ID = 9;

	ComponentId GetId() const override;

	int GetPlayerScore();
	void SetPlayerScore(int playerScore);

	int GetEnemyScore();
	void SetEnemyScore(int enemyScore);

private:
	int mPlayerScore;
	int mEnemyScore;
};
