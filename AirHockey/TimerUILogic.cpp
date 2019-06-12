#include "pch.h"
#include "TimerUILogic.h"
#include "TimerUIData.h"
#include "EventCenter.h"
#include "GameObject.h"

Component::ComponentId TimerUILogic::GetId() const
{
	return AIComponent::COMPONENT_ID;
}

void TimerUILogic::Update(GameObject& gameObject, float deltaTime)
{
	TimerUIData* UIData = gameObject.GetComponent<TimerUIData>(TimerUIData::COMPONENT_ID);
	UIData->SetTime(std::max(0.0f, UIData->GetTime() - deltaTime));

	if (UIData->GetTime() == 0.0f)
		EventCenter::GetInstance().Send(std::make_unique<Event>(Event::PLAY_TIME_FINISHED_ID, gameObject.GetId()));
}