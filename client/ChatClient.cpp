#include <iostream>
#define SSL_R_SHORT_READ 219
#include <boost/bind.hpp>
#include "ChatClient.h"

ChatClient::ChatClient(boost::asio::io_service& io_service, ssl::context& context,
    const std::string& host, const std::string& tcpPort, const std::string& nickname, int peerPort)
    :
    m_socket(io_service, context),
    m_resolver(io_service),
    m_nickname(nickname),
    m_port(tcpPort),
    m_host(host),
    m_isConnect(false),
    m_peer(io_service, peerPort)
{
    m_socket.set_verify_mode(ssl::verify_peer);
    m_socket.set_verify_callback(boost::bind(&ChatClient::VerifyCertificate, this, _1, _2));
}

bool ChatClient::VerifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx)
{
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);

    return preverified;
}

void ChatClient::Start()
{
    std::cout << "Connect to server..." << std::endl;
    m_peer.DoReceive();
    DoResolve();
}

void ChatClient::DoResolve()
{
    auto callbackResolve = [this](const boost::system::error_code& error, ip::tcp::resolver::iterator iterator)
        {
            if (error)
            {
                std::cerr << "Resolve error: " << error.message() << std::endl;
                DoResolve();
                return;
            }

            DoConnect(iterator);
        };

    m_resolver.async_resolve(m_host, m_port, callbackResolve);
}

void ChatClient::DoConnect(ip::tcp::resolver::iterator iterator)
{
    auto callbackConncect = [this](const boost::system::error_code& error, ip::tcp::resolver::iterator iter)
        {
            if (error)
            {
                std::cerr << "Connect error: " << error.message() << std::endl;
                DoResolve();
                return;
            }

            DoHandshake();
        };

    async_connect(m_socket.lowest_layer(), iterator, callbackConncect);
}

void ChatClient::DoHandshake()
{
    auto callbackHandshake = [this](const boost::system::error_code& error)
        {
            if (!error)
            {
                std::cout << "[Room] " << m_nickname << " Connect" << std::endl;

                m_message.SetType(TypeMessage::NICKNAME);
                std::string temp = m_nickname;
                m_message.SetMsg(temp);
                std::string msg = std::move(m_message.Encode());

                m_message.SetType(TypeMessage::UDPPORT);
                temp = std::to_string(m_peer.GetPort());
                m_message.SetMsg(temp);
                msg += m_message.Encode();;

                DoWrite(msg);
            
                m_isConnect = true;
                DoReceive();
            }
            else
            {
                std::cerr << "Handshake failed: " << error.message() << std::endl;
                DoResolve();
            }
        };

    m_socket.async_handshake(ssl::stream_base::client, callbackHandshake);
}

void ChatClient::DecodeMessageFromBuffer(std::size_t bytesTransferred)
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

void ChatClient::Disconnect()
{
    if (!m_isConnect)
        return;

    auto callbackShutdown = [this](const boost::system::error_code& error)
        {
            m_isConnect = false;

            std::string msg = "[Room] Server close\n[Room] [You] Disconnect\n";
            std::cout << msg;
        };

    m_socket.async_shutdown(callbackShutdown);
}

void ChatClient::GetIP()
{
    if (m_message.GetMsg() == "error")
    {
        std::cerr << "[P2P] Unknown nickname" << std::endl;
        return;
    }

    std::string msg = std::move(m_message.GetMsg());
    int pos = msg.find(":");

    std::string ip = std::move(msg.substr(0, pos));
    int port = stoi(msg.substr(pos + 1, msg.size() - pos));

    m_peer.DoSend(ip, port);

}

void ChatClient::HandleMessage()
{
    switch (m_message.GetType())
    {
    case TypeMessage::GETIP:
        GetIP();
        break;
    case TypeMessage::MSG:
        std::cout << "[Room] " << m_message.GetMsg() << std::endl;
        break;
    default:
        std::cerr << "[Room] " << " Send undefined message" << std::endl;
    }
}

void ChatClient::DoReceive()
{
    auto callbackRead = [this](const boost::system::error_code& error, std::size_t bytesTransferred)
        {
            if (error == boost::asio::error::eof)
            {
                DoReceive();
                return;
            }

            if (error)
            {
                Disconnect();
                return;
            }

            DecodeMessageFromBuffer(bytesTransferred);
            DoReceive();
        };

    async_read_until(m_socket, m_buffer, MSG_DELIMER, callbackRead);
}

void ChatClient::DoWrite(const std::string& msg)
{
    auto callbackWrite = [this](const boost::system::error_code& error, std::size_t bytesTransferred)
        {
            if (error)
            {
                Disconnect();
                return;
            }
        };

    async_write(m_socket, boost::asio::buffer(msg, msg.length()), callbackWrite);
}

bool ChatClient::PrepareMessage(std::string& msg)
{
    if (msg[0] != '/')
    {
        m_message.SetType(TypeMessage::MSG);
        m_message.SetMsg(msg);
        return true;
    }

    if (msg.substr(1, 4) == "p2p:")
    {  
        m_message.SetType(TypeMessage::GETIP);
        int pos = msg.find(' ');
        std::string message = msg.substr(5, pos - 5);
        m_message.SetMsg(message);
        m_peer.SetMessage("2:" + m_nickname + ": " + msg.substr(pos + 1, msg.size() - pos) + '\n');
        return true;
    }

    return false;
}

void ChatClient::DoSend()
{
    std::string message("");
    while (std::getline(std::cin, message))
    {
        if (message == "/q")
        {
            Disconnect();
            break;
        }

        if (message.empty())
            continue;

        if (PrepareMessage(message))
            DoWrite(m_message.Encode());
        else
            std::cerr << "Error command" << std::endl;
    } 
}

