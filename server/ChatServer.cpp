#include <iostream>
#define SSL_R_SHORT_READ 219
#include "ChatServer.h"
#include "ChatSession.h"

ChatServer::ChatServer(io_service& io_service, ssl::context& context, int port)
    :
    m_acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port)),
    m_context(context),
    m_sessions(),
    m_peerPorts()
{
}


void ChatServer::Start()
{
    DoAccept();
    std::cout << "Server start\n";
}

void ChatServer::DoAccept()
{
    auto callbackAccept = [this](const boost::system::error_code& error, ip::tcp::socket socket)
        {
            if (error)
            {
                std::cerr << "Accept error: " << error.message() << std::endl;
                return;
            }

            TcpSocket sslSocket(std::move(socket), m_context);
            auto session = std::make_shared<ChatSession>(std::move(sslSocket), m_sessions, m_peerPorts);
            session->Start();
            DoAccept();
        };

    m_acceptor.async_accept(callbackAccept);
}
