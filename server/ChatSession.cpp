#include <iostream>
#include "ChatSession.h"


ChatSession::ChatSession(TcpSocket socket, MapSession& sessions, MapPeerPorts& peerPorts)
	:
	m_socket(std::move(socket)),
	m_sessions(sessions),
	m_message(),
	m_isConnect(true),
	m_buffer(),
	m_nickname(""),
	m_peerPorts(peerPorts)
{
}

ChatSession::~ChatSession()
{
}

void ChatSession::Start()
{
	DoHadshake();
}

void ChatSession::DoHadshake()
{
	auto callbackHandshake = [self = shared_from_this()](const boost::system::error_code& error)
		{
			if (error)
			{
				std::cerr << "Handshake failed: " << error.message() << std::endl;
				return;
			}

			self->DoRead();
		};

	m_socket.async_handshake(ssl::stream_base::server, 
		callbackHandshake
	);
}

void ChatSession::DoRead()
{
	auto callbackRead = [self = shared_from_this()](const boost::system::error_code& error, std::size_t bytesTransferred)
		{
			if (error == boost::asio::error::eof)
			{
				self->DoRead();
				return;
			}

			if (error)
			{
				self->Disconnect();
				return;
			}
			
			self->DecodeMessageFromBuffer(bytesTransferred);
			self->DoRead();
		};

	async_read_until(m_socket, m_buffer, MSG_DELIMER, callbackRead);
}

void ChatSession::DoWrite(const std::string& msg)
{
	auto callbackWrite = [self = shared_from_this()](const boost::system::error_code& error, std::size_t bytesTransferred)
		{
			if (error)
			{
				self->Disconnect();
				return;
			}
		};

	async_write(m_socket, boost::asio::buffer(msg, msg.length()), callbackWrite);
}

void ChatSession::Disconnect()
{
	if (!m_isConnect)
		return;

	auto callbackShutdown = [self = shared_from_this()](const boost::system::error_code& error)
		{

			self->m_isConnect = false;
			std::string msg = self->m_nickname + " Disconnect";
			std::cout << "[Room] " << msg << std::endl;

			self->m_message.SetType(TypeMessage::MSG);
			self->m_message.SetMsg(msg);

			self->Broadcast(self->m_message.Encode());
			self->m_sessions.erase(self->m_nickname);
			self->m_peerPorts.erase(self->m_nickname);
		};

	m_socket.async_shutdown(callbackShutdown);
}

void ChatSession::HandleGetIP()
{
	std::string tmp{ "error" };
	auto it = m_sessions.find(m_message.GetMsg());
	if (it != m_sessions.end())
		tmp = it->second->GetSocket().next_layer().remote_endpoint().address().to_string() + ":" + m_peerPorts[m_message.GetMsg()];

	m_message.SetMsg(tmp);
	tmp = std::move(m_message.Encode());

	DoWrite(tmp);
}

void ChatSession::HandleUDPPort()
{
	std::string port = std::move(m_message.GetMsg());
	m_peerPorts.insert_or_assign(m_nickname, port);
}

void ChatSession::HandleMSG()
{
	std::string msg = m_nickname + std::string(": ") + m_message.GetMsg();
	std::cout << "[Room] " << msg << std::endl;
	m_message.SetMsg(msg);
	Broadcast(m_message.Encode());
}

void ChatSession::HandleNickname()
{
	m_nickname = std::move(m_message.GetMsg());
	m_sessions.insert_or_assign(m_nickname, shared_from_this());
	std::string msg = m_nickname + " Connect";
	std::cout << "[Room] " << msg << std::endl;

	m_message.SetMsg(msg);
	m_message.SetType(TypeMessage::MSG);

	Broadcast(m_message.Encode());
}

void ChatSession::HandleMessage()
{
	switch (m_message.GetType())
	{
	case TypeMessage::NICKNAME:
		HandleNickname();
		break;
	case TypeMessage::UDPPORT:
		HandleUDPPort();
		break;
	case TypeMessage::MSG:
		HandleMSG();
		break;
	case TypeMessage::GETIP:
		HandleGetIP();
		break;
	default:
		std::cerr << "[Room] " << m_nickname << ": Send undefined message" << std::endl;
	}
}

void ChatSession::Broadcast(const std::string & buffer)
{
	for (auto& [_, session] : m_sessions)
	{
		if (session == shared_from_this())
			continue;
		session->DoWrite(buffer);
	}
}

void ChatSession::DecodeMessageFromBuffer(std::size_t bytesTransferred)
{
	std::istream is(&m_buffer);

	std::string message;
	message.resize(bytesTransferred);
	while (std::getline(is, message))
	{
		m_message.Decode(message);
		HandleMessage();
	}

	m_buffer.consume(bytesTransferred);
}
