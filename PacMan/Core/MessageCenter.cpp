#include "MessageCenter.h"
#include <algorithm>
#include <cassert>

MessageCenter& MessageCenter::GetInstance()
{
	static MessageCenter messageCenter;
	return messageCenter;
}

void MessageCenter::RegisterListener(MessageListener* pListener)
{
	assert(std::find(m_Listeners.begin(), m_Listeners.end(), pListener) == m_Listeners.end());
	m_Listeners.emplace_back(pListener);
}

void MessageCenter::UnregisterListener(MessageListener* pListener)
{
	auto it = std::find(m_Listeners.begin(), m_Listeners.end(), pListener);
	assert(it != m_Listeners.end());
	m_Listeners.erase(it);
}

void MessageCenter::ProcessMessages()
{
	auto messages = std::move(m_Messages);
	for (; !messages.empty(); messages.pop())
	{
		const auto& message = messages.front();
		for (std::size_t i = 0; i < m_Listeners.size(); ++i)
			m_Listeners[i]->OnMessageReceived(message);
	}
}

void MessageCenter::AddMessage(const Message& message)
{
	m_Messages.emplace(message);
}
