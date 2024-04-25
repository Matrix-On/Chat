#include <iostream>
#include <conio.h>
#include <config.h>
#include <Singleton.h>
#include <CommandLine.h>
#include "ChatClient.h"

void PrepareParams();

int main(int argc, char* argv[])
{
    commandLineInfo.processParams(argc, argv);

    try
    {
        PrepareParams();

        boost::asio::io_service io_service;
        boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
        context.load_verify_file("rootca.crt");

        std::string host = commandLineInfo.getParamInfo("sip");
        std::string port = commandLineInfo.getParamInfo("sport");
        std::string nickname = commandLineInfo.getParamInfo("nick");
        int peerPort = stoi(commandLineInfo.getParamInfo("pport"));

        ChatClient client(io_service, context,
            host, port, nickname, peerPort);
        client.Start();
        std::thread th([&client]() { client.DoSend(); });

        io_service.run();
        th.join();
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
    if (commandLineInfo.getParamInfo("sip").empty())
    {
        throw std::runtime_error("Invalid server ip");
    }

    if (commandLineInfo.getParamInfo("sport").empty())
    {
        throw std::runtime_error("Invalid server port");
    }

    if (commandLineInfo.getParamInfo("pport").empty())
    {
        throw std::runtime_error("Invalid peer port");
    }

    if (commandLineInfo.getParamInfo("nick").empty())
    {
        throw std::runtime_error("Invalid nick");
    }
}