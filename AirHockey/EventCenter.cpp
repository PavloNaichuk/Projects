#include "pch.h"
#include "EventCenter.h"

Event::Event(EventId eventId) 
	: mEventId(eventId)
{
}

EventCenter& EventCenter::GetInstance() 
{
	static EventCenter instance;
	return instance;
}

void EventCenter::Update() 
{
	while (!mEvents.empty()) 
	{
		const UniqueEvent& event = mEvents.front();
		
		for (auto& callback : mCallbacks)
			callback(*event);

		mEvents.pop();
	}
}

void EventCenter::Send(UniqueEvent event) 
{
	mEvents.push(std::move(event));
}

void EventCenter::Subscribe(EventCallback callback) 
{
	mCallbacks.push_back(std::move(callback));
}