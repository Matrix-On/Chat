#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <unordered_map>
#include <string>

#define MSG_DELIMER '\n'

#define M_UDP_PORT 54322
#define M_TCP_PORT 12345

class ChatSession;

using namespace boost::asio;

typedef std::unordered_map<std::string, std::shared_ptr<ChatSession>> MapSession;
typedef std::unordered_map<std::string, std::string> MapPeerPorts;
typedef ssl::stream<ip::tcp::socket> TcpSocket;
typedef ip::udp::socket UdpSocket;
typedef ip::tcp::acceptor TcpAccepter;
