#pragma once
#include <ChatMessage.h>
#include <config.h>

class ChatPeer
{
public:
	ChatPeer(boost::asio::io_service& io_service, int port);

	void DoSend(const std::string& ip, int port);
	void DoReceive();

	inline void SetMessage(const std::string& msg) { m_bufMessage = msg; }

	inline int GetPort() { return m_socket.local_endpoint().port(); }

private:
	void DecodeMessageFromBuffer(std::size_t bytesTransferred);
	void HandleMessage();

	UdpSocket m_socket;
	boost::asio::streambuf m_buffer;
	ChatMessage m_message;
	std::string m_bufMessage;
};

