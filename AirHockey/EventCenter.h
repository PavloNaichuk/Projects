#pragma once

#include "Common.h"

struct Event 
{
	enum EventId
	{
		PLAY_TIME_FINISHED_ID,
		GOAL_SCORED_ID,
		PUCK_STRIKER_HIT_ID,
		PUCK_BOARD_WALL_HIT_ID
	};

	Event(EventId eventId, int senderId);

	EventId mEventId;
	int mSenderId;
};

using UniqueEvent = std::unique_ptr<Event>;
using EventCallback = std::function<void (const Event& event)>;

class EventCenter 
{
public:
	EventCenter(const EventCenter&) = delete;
	EventCenter& operator= (const EventCenter&) = delete;

	static EventCenter& GetInstance();

	void Update();
	void Send(UniqueEvent event);
	void Subscribe(EventCallback callback);

private:
	EventCenter() = default;

private:
	std::queue<UniqueEvent> mEvents;
	std::vector<EventCallback> mCallbacks;
};
