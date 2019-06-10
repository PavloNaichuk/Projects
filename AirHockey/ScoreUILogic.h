#pragma once

#include "AIComponent.h"

class ScoreUILogic : public AIComponent
{
public:
	ScoreUILogic(unsigned score);

	float GetScore();
	void SetTScore(unsigned score);

	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime) override;

private:
	float mScore;
};