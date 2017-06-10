#pragma once

#include <string>
#include <vector>
#include <queue>

const char* const PowerUpChanged = "PowerUpChanged";
const char* const PillPointChanged = "PillPointChanged";
const char* const EnterGameRequested = "EnterGameRequested";
const char* const BehaviorChanged = "BehaviorChanged";
const char* const ScoreChanged = "ScoreChanged";
const char* const EntityKilled = "EntityKilled";
const char* const LevelCompleted = "LevelCompleted";
const char* const LevelFailed = "LevelFailed";

struct Message
{
	Message(const char* ID, const void* pSender, const void* pData)
		: m_ID(ID)
		, m_pSender(pSender)
		, m_pData(pData)
	{}
	const std::string m_ID;
	const void* m_pSender;
	const void* m_pData;
};

class MessageListener
{
public:
	virtual ~MessageListener() {}
	virtual void OnMessageReceived(const Message& message) = 0;
};

class MessageCenter
{
public:
	MessageCenter(const MessageCenter&) = delete;
	MessageCenter& operator= (const MessageCenter&) = delete;

	static MessageCenter& GetInstance();

	void RegisterListener(MessageListener* pListener);
	void UnregisterListener(MessageListener* pListener);

	void ProcessMessages();
	void AddMessage(const Message& message);

private:
	MessageCenter() = default;
	~MessageCenter() = default;
	
private:
	std::queue<Message> m_Messages;
	std::vector<MessageListener*> m_Listeners;
};