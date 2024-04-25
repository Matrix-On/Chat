#pragma once
#include <string>

class StrOperations
{
public:
	static bool ExplodeStrToParts(const std::string& str, const std::string& separator, const int& numPart, std::string* part1, ...);

};

