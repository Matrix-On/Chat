#pragma once
#include <unordered_map>
#include <string>

#include "Singleton.h"

class CommandLineInfo
{
public:
    CommandLineInfo();

    void processParams(const int& argc, char* argv[]);
    std::string getParamInfo(const std::string& param);
    void setParamInfo(const std::string& param, const std::string& info);

private:
    friend class Singleton<CommandLineInfo>;

    std::unordered_map<std::string, std::string> m_mapParams;
};

#define commandLineInfo Singleton<CommandLineInfo>::getInstance()
