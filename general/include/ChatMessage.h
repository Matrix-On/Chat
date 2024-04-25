#pragma once
#include <string>

enum class TypeMessage
{
	NOINFO,
	NICKNAME,
	MSG,
	GETIP,
	UDPPORT,
};

class ChatMessage
{
public:
	ChatMessage();

	std::string Encode();
	void Decode(std::string& msg);
	void Clear();

	//inline size_t GetSize() { return m_size; };
	inline TypeMessage GetType() { return m_type; };
	inline std::string& GetMsg() { return m_msg; };

	//inline size_t SetSize();
	inline void SetType(TypeMessage type) { m_type = type; };
	inline void SetMsg(std::string& msg) { m_msg = std::move(msg); };

private:
	//size_t m_size;
	TypeMessage m_type;
	std::string m_msg;
};

