#include "pch.h"
#include "TimerUIData.h"

TimerUIData::TimerUIData(float time) 
	: mTime(time)
{
}

Component::ComponentId TimerUIData::GetId() const
{
	return COMPONENT_ID;
}

float TimerUIData::GetTime() const 
{
	return mTime;
}

void TimerUIData::SetTime(float time) 
{
	mTime = time;
}