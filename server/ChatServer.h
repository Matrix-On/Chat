#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "config.h"

class 
	ChatServer
{
public:
	ChatServer(io_service& io_service, ssl::context& context, int port);

	void Start();

private:
	void DoAccept();

	ssl::context& m_context;
	TcpAccepter m_acceptor;
	MapSession m_sessions;
	MapPeerPorts m_peerPorts;
};

