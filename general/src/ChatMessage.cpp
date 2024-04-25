#include "ChatMessage.h"

ChatMessage::ChatMessage()
{
	Clear();
}

void ChatMessage::Clear()
{
	m_type = TypeMessage::NOINFO;
	m_msg.clear();
}

void ChatMessage::Decode(std::string& msg)
{
	if (msg.empty() || msg.size() == 0)
		return;
	Clear();
	int pos = msg.find(':');
	std::string type = msg.substr(0, pos);
	m_type = (TypeMessage)stoi(type);
	m_msg = msg.substr(pos + 1, msg.length() - type.length());
}

std::string ChatMessage::Encode()
{
	std::string buffer{ std::to_string((int)m_type) + ':' + m_msg + '\n'};
	return std::move(buffer);
}

