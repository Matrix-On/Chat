#include "CommandLine.h"
#include "StrOperations.h"
#include <algorithm>

CommandLineInfo::CommandLineInfo()
{
}

void CommandLineInfo::processParams(const int& argc, char* argv[])
{	
	if (argc == 0)
	{
		return;
	}

	setParamInfo("path", argv[0]);

	for (int i = 1; i < argc; ++i)
	{
		if (*(argv[i]) != '-')
		{
			continue;
		}

		std::string param = "";
		std::string value = "";

		StrOperations::ExplodeStrToParts(argv[i] + 1, "=", 2, &param, &value);

		if (param.empty() || value.empty())
		{
			continue;
		}

		setParamInfo(param, value);
	}
}

std::string CommandLineInfo::getParamInfo(const std::string& param)
{
	std::string lower = param;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return m_mapParams[lower];
}

void CommandLineInfo::setParamInfo(const std::string& param, const std::string& info)
{
	std::string lower = param;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	m_mapParams[lower] = info;
}
