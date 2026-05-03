#pragma once

#include "Component.h"

class TimerUIData : public Component 
{
public:
	TimerUIData(float time);

	static const ComponentId COMPONENT_ID = 10;

	ComponentId GetId() const override;

	float GetTime() const;
	void SetTime(float time);

private:
	float mTime;
};
