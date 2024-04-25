#pragma once
#include <ChatMessage.h>
#include <config.h>
#include "ChatPeer.h"

class ChatClient
{
public:
	ChatClient(boost::asio::io_service& io_service, ssl::context& context, const std::string& host,
		const std::string& tcpPort, const std::string& nickname, int peerPort);
	void Start();
	void DoSend();

protected:
	bool VerifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx);

	void DoResolve();
	void DoHandshake();
	void DoConnect(ip::tcp::resolver::iterator iterator);
	void DoReceive();
	void Disconnect();

	void DoWrite(const std::string& msg);

	void DecodeMessageFromBuffer(std::size_t bytesTransferred);
	void HandleMessage();
	bool PrepareMessage(std::string& msg);

	void GetIP();

	ChatPeer m_peer;

	TcpSocket m_socket;
	ip::tcp::resolver m_resolver;
	boost::asio::streambuf m_buffer;
	ChatMessage m_message;

	std::string m_nickname;
	std::string m_host;
	std::string m_port;
	bool m_isConnect;
};

