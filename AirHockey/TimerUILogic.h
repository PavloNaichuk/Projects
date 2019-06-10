#pragma once

#include"AIComponent.h"

class TimerUILogic : public AIComponent
{
public:
	TimerUILogic(float time);

	float GetTime();
	void SetTime(float time);

	ComponentId GetId() const override;
	void Update(GameObject& gameObject, float deltaTime) override;

private:
	float mTime;
};
