#include <iostream>
#include <conio.h>
#include <boost/bind.hpp>
#include <config.h>
#include <Singleton.h>
#include <CommandLine.h>
#include "ChatServer.h"

void PrepareParams();

std::string get_password()
{
    return "1234";
}


int main(int argc, char* argv[])
{
    commandLineInfo.processParams(argc, argv);

    try
    {
        PrepareParams();

        boost::asio::io_service io_service;

        boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
        context.set_options(boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::single_dh_use);
        context.set_password_callback(boost::bind(get_password));
        context.use_certificate_chain_file("user.crt");
        context.use_private_key_file("user.key", boost::asio::ssl::context::pem);
        context.use_tmp_dh_file("dh2048.pem");

        int port = stoi(commandLineInfo.getParamInfo("sport"));

        ChatServer server(io_service, context, port);
        server.Start();
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    getch();
    return 0;
}


void PrepareParams()
{
    if (commandLineInfo.getParamInfo("sport").empty())
    {
        throw std::runtime_error("Invalid server port");
    }
}
