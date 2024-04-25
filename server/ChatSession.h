#pragma once
#include <ChatMessage.h>
#include <config.h>

class ChatSession : public std::enable_shared_from_this<ChatSession>
{
public:
	ChatSession(TcpSocket socket, MapSession& sessions, MapPeerPorts& peerPorts);
	~ChatSession();

	void Start();

	inline TcpSocket& GetSocket() { return m_socket; }

protected:
	void DoHadshake();
	void DoRead();
	void DoWrite(const std::string& msg);
	void Disconnect();
	void Broadcast(const std::string& buffer);


	void DecodeMessageFromBuffer(std::size_t bytesTransferred);
	void HandleMessage();

	void HandleNickname();
	void HandleMSG();
	void HandleGetIP();
	void HandleUDPPort();



	bool m_isConnect;

	boost::asio::streambuf m_buffer;
	ChatMessage m_message;

	std::string m_nickname;


	TcpSocket m_socket;
	MapSession& m_sessions;
	MapPeerPorts& m_peerPorts;
};

