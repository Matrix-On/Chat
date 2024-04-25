#include <iostream>
#include "ChatPeer.h"

ChatPeer::ChatPeer(boost::asio::io_service& io_service, int port)
	:
	m_socket(io_service, ip::udp::endpoint(ip::udp::v4(), port)),
    m_bufMessage(),
    m_message(),
    m_buffer()
{
}

void ChatPeer::DoSend(const std::string& ip, int port)
{
    auto callbackSend = [this](const boost::system::error_code& error, std::size_t bytesTransferred)
        {
            if (error)
            {
                std::cerr << "[P2P] Error send!" << std::endl;
            }
        };

    ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

    m_socket.async_send_to(boost::asio::buffer(std::move(m_bufMessage)), endpoint, callbackSend);
}

void ChatPeer::DoReceive()
{  
    auto callbackReceive = [this](const boost::system::error_code& error, std::size_t bytesTransferred)
        {
            if (error == boost::asio::error::eof)
            {
                DoReceive();
                return;
            }

            if (error)
            {
                std::cerr << "[P2P] Error receive!" << std::endl;
                return;
            }

            DecodeMessageFromBuffer(bytesTransferred);
            DoReceive();
        };

    m_socket.async_receive(m_buffer.prepare(1024), callbackReceive);
}

void ChatPeer::DecodeMessageFromBuffer(std::size_t bytesTransferred)
{
    m_buffer.commit(bytesTransferred);
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

void ChatPeer::HandleMessage()
{
    switch (m_message.GetType())
    {
    case TypeMessage::MSG:
        std::cout << "[P2P] " << m_message.GetMsg() << std::endl;
        break;
    default:
        std::cerr << "[P2P] " << " Send undefined message" << std::endl;
    }
}