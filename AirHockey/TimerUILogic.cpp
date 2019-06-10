#include "pch.h"
#include "TimerUILogic.h"
#include "EventCenter.h"

TimerUILogic::TimerUILogic(float time) 
	: mTime(time)
{}

float TimerUILogic::GetTime() 
{
	return mTime;
}

void TimerUILogic::SetTime(float time) 
{
	mTime = time;
}

Component::ComponentId TimerUILogic::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void TimerUILogic::Update(GameObject& gameObject, float deltaTime)
{
	mTime -= deltaTime;
	if (mTime < 0.0f) 
	{
		mTime = 0.0f;
		EventCenter::GetInstance().Send(std::make_unique<Event>(Event::PLAY_TIME_FINISHED_ID));
	}
}